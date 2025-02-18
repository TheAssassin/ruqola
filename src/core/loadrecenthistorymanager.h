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

#pragma once

#include "libruqola_private_export.h"
#include <QDateTime>
#include <qglobal.h>

#define Q_DISABLE_COPY_MOVE(Class)                                                                                                                             \
private:                                                                                                                                                       \
    Class(const Class &);                                                                                                                                      \
    Class &operator=(const Class &);                                                                                                                           \
    Class(const Class &&);                                                                                                                                     \
    Class &operator=(const Class &&);

class LIBRUQOLACORE_TESTS_EXPORT LoadRecentHistoryManager
{
public:
    LoadRecentHistoryManager();
    ~LoadRecentHistoryManager();

    Q_REQUIRED_RESULT qint64 lastLoadingTimeStamp() const;

    Q_REQUIRED_RESULT qint64 generateNewStartTimeStamp(qint64 lastTimeStamp);

private:
    Q_DISABLE_COPY(LoadRecentHistoryManager)
    qint64 mLastLoadingTimeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
};

