/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "managechannels.h"

#include "channelgroupbasejob.h"
#include "channels/channeljoinjob.h"
#include "channels/channelopenjob.h"
#include "groups/groupopenjob.h"
#include "restapirequest.h"
#include "ruqola.h"
#include "ruqola_debug.h"

ManageChannels::ManageChannels(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
}

ManageChannels::~ManageChannels()
{
}

ManageChannels::SearchChannelFound ManageChannels::searchOpenChannels(const QString &roomId)
{
    ManageChannels::SearchChannelFound result = ManageChannels::SearchChannelFound::NoFound;
    for (int roomIdx = 0, nRooms = mAccount->roomModel()->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = mAccount->roomModel()->index(roomIdx, 0);
        const auto identifier = roomModelIndex.data(RoomModel::RoomId).toString();
        if (identifier == roomId) {
            if (roomModelIndex.data(RoomModel::RoomOpen).toBool()) {
                result = ManageChannels::SearchChannelFound::ChannelOpened;
                Q_EMIT selectRoomByRoomIdRequested(roomId);
            } else {
                result = ManageChannels::SearchChannelFound::ChannelHidden;
            }
            break;
        }
    }
    return result;
}

RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo ManageChannels::generateGroupInfo(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo)
{
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
    switch (typeInfo) {
    case RocketChatAccount::ChannelTypeInfo::RoomId:
        info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
        break;
    case RocketChatAccount::ChannelTypeInfo::RoomName:
        info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Name;
        break;
    }
    info.identifier = roomId;
    return info;
}

void ManageChannels::channelJoin(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo, const QString &joinCode)
{
    auto job = new RocketChatRestApi::ChannelJoinJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setJoinCode(joinCode);
    job->setChannelGroupInfo(channelInfo);
    connect(job, &RocketChatRestApi::ChannelJoinJob::setChannelJoinDone, this, &ManageChannels::setChannelJoinDone);
    connect(job, &RocketChatRestApi::ChannelJoinJob::missingChannelPassword, this, &ManageChannels::missingChannelPassword);
    connect(job, &RocketChatRestApi::ChannelJoinJob::openArchivedRoom, this, &ManageChannels::openArchivedRoom);
    if (!job->start()) {
        qCDebug(RUQOLA_LOG) << "Impossible to start setChannelJoin";
    }
}

void ManageChannels::openPrivateGroup(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo)
{
    const ManageChannels::SearchChannelFound result = searchOpenChannels(roomId);
    if (result == ManageChannels::SearchChannelFound::ChannelOpened) {
        return;
    }

    const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info = generateGroupInfo(roomId, typeInfo);
    qCDebug(RUQOLA_LOG) << "opening group" << roomId;
    switch (result) {
    case ManageChannels::SearchChannelFound::ChannelOpened:
        // Nothing
        break;
    case ManageChannels::SearchChannelFound::NoFound: {
        // TODO verify
        channelJoin(info, QString());
        break;
    }
    case ManageChannels::SearchChannelFound::ChannelHidden: {
        auto job = new RocketChatRestApi::GroupOpenJob(this);
        job->setChannelGroupInfo(info);
        mAccount->restApi()->initializeRestApiJob(job);
        connect(job,
                &RocketChatRestApi::GroupOpenJob::groupOpenDone,
                this,
                [this](const QJsonObject &, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo) {
                    Q_EMIT selectRoomByRoomIdRequested(channelInfo.identifier);
                });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start GroupOpenJob job";
        }
        break;
    }
    }
}

void ManageChannels::openChannel(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo)
{
    const ManageChannels::SearchChannelFound result = searchOpenChannels(roomId);
    if (result == ManageChannels::SearchChannelFound::ChannelOpened) {
        return;
    }
    const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info = generateGroupInfo(roomId, typeInfo);
    qCDebug(RUQOLA_LOG) << "opening channel" << roomId;
    switch (result) {
    case ManageChannels::SearchChannelFound::ChannelOpened:
        // Nothing
        break;
    case ManageChannels::SearchChannelFound::NoFound: {
        channelJoin(info, QString());
        break;
    }
    case ManageChannels::SearchChannelFound::ChannelHidden: {
        auto job = new RocketChatRestApi::ChannelOpenJob(this);
        job->setChannelGroupInfo(info);
        mAccount->restApi()->initializeRestApiJob(job);
        connect(job,
                &RocketChatRestApi::ChannelOpenJob::channelOpenDone,
                this,
                [this](const QJsonObject &, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo) {
                    Q_EMIT selectRoomByRoomIdRequested(channelInfo.identifier);
                });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start ChannelOpenJob job";
        }
        break;
    }
    }
}

void ManageChannels::setChannelJoinDone(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    mAccount->ddp()->subscribeRoomMessage(channelInfo.identifier);
    // FIXME room is not added yet...
    switch (channelInfo.channelGroupInfoType) {
    case RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Unknown:
        qCWarning(RUQOLA_LOG) << "setChannelJoinDone : RocketChatRestApi::ChannelBaseJob::ChannelInfoType::Unknown";
        break;
    case RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier:
        Q_EMIT selectRoomByRoomIdRequested(channelInfo.identifier);
        break;
    case RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Name:
        Q_EMIT selectRoomByRoomNameRequested(channelInfo.identifier);
        break;
    }
}
