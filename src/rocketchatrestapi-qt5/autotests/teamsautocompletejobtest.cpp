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

#include "teamsautocompletejobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamsautocompletejob.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(TeamsAutoCompleteJobTest)
using namespace RocketChatRestApi;
TeamsAutoCompleteJobTest::TeamsAutoCompleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsAutoCompleteJobTest::shouldHaveDefaultValue()
{
    TeamsAutoCompleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.name().isEmpty());
}

void TeamsAutoCompleteJobTest::shouldGenerateRequest()
{
    TeamsAutoCompleteJob job;
    const QString name = QStringLiteral("blu");
    job.setName(name);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.autocomplete?name=%1").arg(name)));
}

void TeamsAutoCompleteJobTest::shouldNotStarting()
{
    TeamsAutoCompleteJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString name = QStringLiteral("foo1");
    job.setName(name);
    QVERIFY(job.canStart());
}
