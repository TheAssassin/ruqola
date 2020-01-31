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

#include "messagelineedit.h"
#include "rocketchataccount.h"
#include "model/inputcompletermodel.h"
#include "ruqola.h"

#include <QKeyEvent>
#include <QListView>
#include <QScreen>
#include <QScrollBar>

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <QApplication>
#include <QDesktopWidget>
#endif

MessageLineEdit::MessageLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
{
    connect(this, &MessageLineEdit::returnPressed, this, [this]() {
        Q_EMIT sendMessage(text());
        clear();
    });
    connect(this, &QLineEdit::textChanged, this, &MessageLineEdit::slotTextChanged);
    setCompletionModel(Ruqola::self()->rocketChatAccount()->inputCompleterModel());
    connect(this, &MessageLineEdit::complete, this, &MessageLineEdit::slotComplete);
}

MessageLineEdit::~MessageLineEdit()
{
}

void MessageLineEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        e->ignore();
        Q_EMIT clearNotification();
        return;
    default:
        break;
    }
    CompletionLineEdit::keyPressEvent(e);
}

void MessageLineEdit::slotTextChanged(const QString &text)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    rcAccount->setInputTextChanged(text, cursorPosition());
}

void MessageLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(InputCompleterModel::CompleterName).toString();
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString newText = rcAccount->replaceWord(completerName + QLatin1Char(' '), text(), cursorPosition());

    mCompletionListView->hide();

    disconnect(this, &QLineEdit::textChanged, this, &MessageLineEdit::slotTextChanged);
    setText(newText);
    connect(this, &QLineEdit::textChanged, this, &MessageLineEdit::slotTextChanged);
}
