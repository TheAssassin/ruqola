/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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

#include <QObject>

class RoomModelTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomModelTest(QObject *parent = nullptr);
    ~RoomModelTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldReturnRowCount();
    void shouldFindRoom();
    void shouldAddRoom();
    void shouldUpdateRoom();
    void shouldUpdateRoomFromQJsonObject();
    void shouldUpdateSubcriptionActionRemoved();
    void shouldUpdateSubcriptionActionInserted();
    void shouldUpdateSubcriptionActionUpdated();
    void shouldClear();
    void shouldReset();
    void shouldReturnDataDefault();
    void shouldReturnData();
    void shouldInsertRoom_data();
    void shouldInsertRoom();
    void shouldOrderRooms();
};

