/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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


#include "customuserstatusestest.h"
#include "customuserstatuses.h"
#include "customuserstatus.h"
#include "ruqola_autotest_helper.h"
#include <QJsonDocument>

#include <QTest>
QTEST_GUILESS_MAIN(CustomUserStatusesTest)

CustomUserStatusesTest::CustomUserStatusesTest(QObject *parent)
    : QObject(parent)
{

}

void CustomUserStatusesTest::shouldHaveDefaultValues()
{
    CustomUserStatuses w;
    QVERIFY(w.customUserses().isEmpty());
    QVERIFY(w.isEmpty());
    QCOMPARE(w.offset(), 0);
    QCOMPARE(w.count(), 0);
    QCOMPARE(w.total(), 0);
}

void CustomUserStatusesTest::shouldLoadUserCustomStatuses_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("customStatusesCount");
    QTest::addColumn< QVector<CustomUserStatus> >("customUserStatus");

    {
        QVector<CustomUserStatus> result;
        {
            CustomUserStatus f;
            f.setIdentifier(QStringLiteral("GZHpA5fENrWHRfaoN"));
            f.setName(QStringLiteral("CM"));
            f.setStatusType(QStringLiteral("busy"));
            f.setUpdatedAt(1603106197911);
            result << f;
        }
        {
            CustomUserStatus f;
            f.setIdentifier(QStringLiteral("tygCbhbgCojk8G28G"));
            f.setName(QStringLiteral("Vacation"));
            f.setStatusType(QStringLiteral("away"));
            f.setUpdatedAt(1588199612532);
            result << f;
        }
        QTest::addRow("customuser2") << QStringLiteral("customuser2") << 2 << result;
    }
}

void CustomUserStatusesTest::shouldLoadUserCustomStatuses()
{
    QFETCH(QString, name);
    QFETCH(int, customStatusesCount);
    QFETCH(QVector<CustomUserStatus>, customUserStatus);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/customuserstatus/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    CustomUserStatuses r;
    r.parseCustomUserStatuses(obj);
    bool compareCustom = r.customUserses() == customUserStatus;
    if (!compareCustom) {
        qDebug() << "current : " << r.customUserses();
        qDebug() << "expected: " << customUserStatus;
    }
    QVERIFY(compareCustom);
    QCOMPARE(r.count(), customStatusesCount);
}
