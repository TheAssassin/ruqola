/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#ifndef GETCHANNELROLESJOB_H
#define GETCHANNELROLESJOB_H

#include "librestapi_private_export.h"
#include "channelbasejob.h"
class QNetworkRequest;
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetChannelRolesJob : public ChannelBaseJob
{
    Q_OBJECT
public:
    explicit GetChannelRolesJob(QObject *parent = nullptr);
    ~GetChannelRolesJob() override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
protected:
    QString jobName() const override;

Q_SIGNALS:
    void channelRolesDone(const QJsonObject &obj, const ChannelBaseJob::ChannelInfo &channelInfo);

private:
    Q_DISABLE_COPY(GetChannelRolesJob)
    void slotGetChannelRolesFinished();
};
}
#endif // GETCHANNELROLESJOB_H
