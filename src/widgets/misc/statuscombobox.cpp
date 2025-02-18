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

#include "statuscombobox.h"
#include "model/statusmodel.h"
#include "model/statusmodelfilterproxymodel.h"

StatusCombobox::StatusCombobox(QWidget *parent)
    : QComboBox(parent)
{
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

StatusCombobox::~StatusCombobox()
{
}

User::PresenceStatus StatusCombobox::status() const
{
    return currentData(StatusModel::Status).value<User::PresenceStatus>();
}

void StatusCombobox::setStatus(User::PresenceStatus status, const QString &customText)
{
    if (customText.isEmpty()) {
        setCurrentIndex(findData(QVariant::fromValue(status), StatusModel::Status));
    } else {
        // TODO use customText
        setCurrentIndex(findData(QVariant::fromValue(status), StatusModel::Status));
    }
}

void StatusCombobox::setUseOnlyStandardStatus()
{
    auto statusProxyModel = new StatusModelFilterProxyModel(this);
    statusProxyModel->setUseOnlyStandardStatus(true);
    statusProxyModel->setSourceModel(new StatusModel(this));
    setModel(statusProxyModel);
}
