/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "administratorserverinfowidget.h"
#include "misc/statisticsjob.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "serverinfojob.h"

#include <KFormat>
#include <KLocalizedString>
#include <KTreeWidgetSearchLineWidget>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMenu>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

AdministratorServerInfoWidget::AdministratorServerInfoWidget(QWidget *parent)
    : QWidget(parent)
    , mTreeWidget(new QTreeWidget(this))
    , mSearchLineWidget(new KTreeWidgetSearchLineWidget(this, mTreeWidget))
    , mRefreshButton(new QPushButton(i18n("Refresh"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);
    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    hboxLayout->addWidget(mSearchLineWidget);
    mRefreshButton->setObjectName(QStringLiteral("mRefreshButton"));
    hboxLayout->addWidget(mRefreshButton);
    connect(mRefreshButton, &QPushButton::clicked, this, &AdministratorServerInfoWidget::slotRefreshInfo);

    mTreeWidget->setObjectName(QStringLiteral("mTreeWidget"));
    mTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mTreeWidget, &QTreeWidget::customContextMenuRequested, this, [this](const QPoint &pos) {
        slotContextMenu(pos);
    });

    mainLayout->addWidget(mTreeWidget);
    mTreeWidget->header()->hide();
    mTreeWidget->setColumnCount(2);
    initialize();
}

AdministratorServerInfoWidget::~AdministratorServerInfoWidget()
{
}

void AdministratorServerInfoWidget::slotContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeWidget->indexAt(pos);
    menu.addAction(i18n("Copy"), this, [index]() {
        const QString currentValue = index.data().toString();
        QClipboard *clip = QApplication::clipboard();
        clip->setText(currentValue, QClipboard::Clipboard);
        clip->setText(currentValue, QClipboard::Selection);
    });
    menu.exec(mTreeWidget->viewport()->mapToGlobal(pos));
}

void AdministratorServerInfoWidget::slotRefreshInfo()
{
    loadStatisticInfo(true);
}

void AdministratorServerInfoWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto serverInfoJob = new RocketChatRestApi::ServerInfoJob(this);
    serverInfoJob->setForceRequiresAuthentication(true);
    rcAccount->restApi()->initializeRestApiJob(serverInfoJob);
    connect(serverInfoJob, &RocketChatRestApi::ServerInfoJob::serverInfoDone, this, &AdministratorServerInfoWidget::slotServerInfoDone);
    if (!serverInfoJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ServerInfoJob";
    }
}

void AdministratorServerInfoWidget::slotServerInfoDone(const QString &versionInfo, const QJsonObject &obj)
{
    Q_UNUSED(versionInfo)
    // qDebug() << " obj " << obj;
    mServerInfo.parseServerInfo(obj);
    // qDebug() << " info " << mServerInfo;
    loadStatisticInfo(false);
    loadServerInfo();
}

void AdministratorServerInfoWidget::loadStatisticInfo(bool refresh)
{
    auto statisticJob = new RocketChatRestApi::StatisticsJob(this);
    statisticJob->setRefresh(refresh);
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    rcAccount->restApi()->initializeRestApiJob(statisticJob);
    connect(statisticJob, &RocketChatRestApi::StatisticsJob::statisticDone, this, &AdministratorServerInfoWidget::slotStatisticDone);
    if (!statisticJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start StatisticsJob";
    }
}

void AdministratorServerInfoWidget::parseServerInfo(QTreeWidgetItem *serverInfoItem, const QJsonObject &obj)
{
    createItemFromStringValue(serverInfoItem, obj, i18n("Version"), QStringLiteral("version"));
    createItemFromStringValue(serverInfoItem, obj, i18n("Deployment ID"), QStringLiteral("uniqueId"));
}

void AdministratorServerInfoWidget::createItemFromStringValue(QTreeWidgetItem *parentItem,
                                                              const QJsonObject &obj,
                                                              const QString &label,
                                                              const QString &identifier)
{
    const QJsonValue objValue = obj.value(identifier);
    if (!objValue.isUndefined()) {
        auto item = new QTreeWidgetItem(parentItem);
        item->setText(0, label);
        item->setText(1, objValue.toString());
        item->addChild(item);
    }
}

void AdministratorServerInfoWidget::parseUsageInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj)
{
    createItemFromIntValue(usageInfoItem, obj, i18n("Rocket.Chat App Users"), QStringLiteral("appUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Away Users"), QStringLiteral("awayUsers"));
    createItemDisplayFormat(usageInfoItem, obj, i18n("Total Uploads Size"), QStringLiteral("uploadsTotalSize"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Offline Users"), QStringLiteral("offlineUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Online Users"), QStringLiteral("onlineUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Activated Users"), QStringLiteral("activeUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Activated Guests"), QStringLiteral("activeGuests"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Uploads"), QStringLiteral("uploadsTotal"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Channels"), QStringLiteral("totalChannels"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Rooms"), QStringLiteral("totalRooms"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Private Groups"), QStringLiteral("totalPrivateGroups"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Threads"), QStringLiteral("totalThreads"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Users"), QStringLiteral("totalUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages"), QStringLiteral("totalMessages"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages in Channels"), QStringLiteral("totalChannelMessages"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages in Private Groups"), QStringLiteral("totalPrivateGroupMessages"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages in Direct Messages"), QStringLiteral("totalDirectMessages"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Discussions"), QStringLiteral("totalDiscussions"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Connected Users"), QStringLiteral("totalConnectedUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Deactivated Users"), QStringLiteral("nonActiveUsers"));
}

void AdministratorServerInfoWidget::createItemFromIntValue(QTreeWidgetItem *usageInfoItem,
                                                           const QJsonObject &obj,
                                                           const QString &label,
                                                           const QString &identifier)
{
    const QJsonValue totalDirectMessages = obj.value(identifier);
    if (!totalDirectMessages.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, label);
        item->setText(1, QString::number(totalDirectMessages.toInt()));
        usageInfoItem->addChild(item);
    }
}

void AdministratorServerInfoWidget::createItemDisplayFormat(QTreeWidgetItem *parentItem,
                                                            const QJsonObject &obj,
                                                            const QString &label,
                                                            const QString &identifier)
{
    const QJsonValue objValue = obj.value(identifier);
    if (!objValue.isUndefined()) {
        auto item = new QTreeWidgetItem(parentItem);
        item->setText(0, label);
        item->setText(1, KFormat().formatByteSize(objValue.toDouble()));
        item->addChild(item);
    }
}

void AdministratorServerInfoWidget::parseRuntimeInfo(QTreeWidgetItem *runtimeInfoItem, const QJsonObject &obj)
{
    const QJsonObject runtimeObj = obj.value(QLatin1String("os")).toObject();
    createItemFromStringValue(runtimeInfoItem, runtimeObj, i18n("OS Release"), QStringLiteral("release"));
    createItemFromStringValue(runtimeInfoItem, runtimeObj, i18n("OS Type"), QStringLiteral("type"));
    createItemFromStringValue(runtimeInfoItem, runtimeObj, i18n("OS Platform"), QStringLiteral("platform"));
    createItemDisplayFormat(runtimeInfoItem, runtimeObj, i18n("OS Total Memory"), QStringLiteral("totalmem"));
    createItemDisplayFormat(runtimeInfoItem, runtimeObj, i18n("OS Free Memory"), QStringLiteral("freemem"));
}

void AdministratorServerInfoWidget::parseCommitInfo(QTreeWidgetItem *commitInfoItem)
{
    {
        auto item = new QTreeWidgetItem(commitInfoItem);
        item->setText(0, i18n("Hash"));
        item->setText(1, mServerInfo.commitHash());
        item->addChild(item);
    }
    {
        auto item = new QTreeWidgetItem(commitInfoItem);
        item->setText(0, i18n("Branch"));
        item->setText(1, mServerInfo.commitBranch());
        item->addChild(item);
    }
    {
        auto item = new QTreeWidgetItem(commitInfoItem);
        item->setText(0, i18n("Tag"));
        item->setText(1, mServerInfo.commitTag());
        item->addChild(item);
    }
    {
        auto item = new QTreeWidgetItem(commitInfoItem);
        item->setText(0, i18n("Author"));
        item->setText(1, mServerInfo.commitAuthor());
        item->addChild(item);
    }
    {
        auto item = new QTreeWidgetItem(commitInfoItem);
        item->setText(0, i18n("Subject"));
        item->setText(1, mServerInfo.commitSubject());
        item->addChild(item);
    }
}

void AdministratorServerInfoWidget::parseBuildInfo(QTreeWidgetItem *buildInfoItem)
{
    {
        auto item = new QTreeWidgetItem(buildInfoItem);
        item->setText(0, i18n("OS Release"));
        item->setText(1, mServerInfo.osRelease());
        item->addChild(item);
    }
    {
        auto item = new QTreeWidgetItem(buildInfoItem);
        item->setText(0, i18n("Node Version"));
        item->setText(1, mServerInfo.nodeVersion());
        item->addChild(item);
    }
    {
        auto item = new QTreeWidgetItem(buildInfoItem);
        item->setText(0, i18n("Arch"));
        item->setText(1, mServerInfo.arch());
        item->addChild(item);
    }
    {
        auto item = new QTreeWidgetItem(buildInfoItem);
        item->setText(0, i18n("OS Platform"));
        item->setText(1, mServerInfo.platform());
        item->addChild(item);
    }
}

void AdministratorServerInfoWidget::slotStatisticDone(const QJsonObject &obj)
{
    // qDebug() << "AdministratorServerInfoWidget::slotStatisticDone " << obj;
    delete mServerInfoItem;
    mServerInfoItem = new QTreeWidgetItem(mTreeWidget);
    mServerInfoItem->setText(0, i18n("Server Info"));
    parseServerInfo(mServerInfoItem, obj);

    delete mUsageInfoItem;
    mUsageInfoItem = new QTreeWidgetItem(mTreeWidget);
    mUsageInfoItem->setText(0, i18n("Usage"));
    parseUsageInfo(mUsageInfoItem, obj);

    delete mRuntimeInfoItem;
    mRuntimeInfoItem = new QTreeWidgetItem(mTreeWidget);
    mRuntimeInfoItem->setText(0, i18n("Runtime Environment"));
    parseRuntimeInfo(mRuntimeInfoItem, obj);
    mTreeWidget->expandAll();
    mTreeWidget->resizeColumnToContents(1);
    mTreeWidget->resizeColumnToContents(0);
}

void AdministratorServerInfoWidget::loadServerInfo()
{
    auto commitItem = new QTreeWidgetItem(mTreeWidget);
    commitItem->setText(0, i18n("Commit"));
    parseCommitInfo(commitItem);

    auto buildItem = new QTreeWidgetItem(mTreeWidget);
    buildItem->setText(0, i18n("Build Environment"));
    parseBuildInfo(buildItem);
}
