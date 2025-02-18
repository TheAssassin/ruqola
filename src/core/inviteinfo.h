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

#pragma once

#include "libruqolacore_export.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT InviteInfo
{
public:
    InviteInfo();
    ~InviteInfo() = default;
    void parseInviteInfo(const QJsonObject &replyObject);
    const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    int maxUses() const;
    void setMaxUses(int newMaxUses);

    const QString &userIdentifier() const;
    void setUserIdentifier(const QString &newUserIdentifier);

    const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    int uses() const;
    void setUses(int newUses);

    const QDateTime &expireDateTime() const;
    void setExpireDateTime(const QDateTime &newExpireDateTime);

    const QDateTime &createDateTime() const;
    void setCreateDateTime(const QDateTime &newCreateDateTime);

    Q_REQUIRED_RESULT bool operator==(const InviteInfo &other) const;

private:
    QString mUserIdentifier;
    QString mIdentifier;
    QString mRoomId;
    QDateTime mExpireDateTime;
    QDateTime mCreateDateTime;
    int mUses = 0;
    int mMaxUses = 0;
};

Q_DECLARE_METATYPE(InviteInfo)
Q_DECLARE_TYPEINFO(InviteInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const InviteInfo &t);
