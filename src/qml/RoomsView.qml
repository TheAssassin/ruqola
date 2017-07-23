/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

import QtQuick 2.0
import org.kde.kirigami 2.1 as Kirigami
import KDE.Ruqola.Ruqola 1.0

ListView {
    id: roomsList
    
    property string selectedRoomID;
    
    signal roomSelected(string roomID)
    header: Kirigami.Heading {
        text: "Hello, " + Ruqola.userName
        width: roomsList.width
        wrapMode: Text.WordWrap
    }
    delegate: RoomDelegate {
        width: roomsList.width
        d_name: name
        d_unread: unread
        d_roomID: room_id
        d_selected: selectedRoomID == room_id
//         d_selected: selected
        
        onRoomSelected : {
            roomsList.roomSelected(roomID)
        }
    }
}
