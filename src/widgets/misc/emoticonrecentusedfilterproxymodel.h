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

#ifndef EMOTICONRECENTUSEDFILTERPROXYMODEL_H
#define EMOTICONRECENTUSEDFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT EmoticonRecentUsedFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EmoticonRecentUsedFilterProxyModel(QObject *parent = nullptr);
    ~EmoticonRecentUsedFilterProxyModel() override;

    Q_REQUIRED_RESULT QStringList usedIdentifier() const;
    void setUsedIdentifier(const QStringList &usedIdentifier);

    void addIdentifier(const QString &identifier);

    void loadRecentUsed();
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    void writeRecentUsed();
    QStringList mUsedIdentifier;
};

#endif // EMOTICONRECENTUSEDFILTERPROXYMODEL_H
