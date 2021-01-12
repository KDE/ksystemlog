/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "statusBar.h"

#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTime>

#include <QIcon>

#include <KComboBox>
#include <KLocalizedString>
#include <KSqueezedTextLabel>

#include "logging.h"
using namespace KSystemLog;
StatusBar::StatusBar(QWidget *parent)
    : QStatusBar(parent)
{
    mLineCountLabel = new QLabel(this);
    mLineCountLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mLineCountLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mLineCountLabel->setLineWidth(2);
    mLineCountLabel->setMidLineWidth(2);
    addPermanentWidget(mLineCountLabel, 1);
    mMessageList = new QComboBox(this);
    mMessageList->setInsertPolicy(QComboBox::InsertAtTop);
    mMessageList->setMaxVisibleItems(5);
    connect(mMessageList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StatusBar::selectLastHistory);
    addPermanentWidget(mMessageList, 4);

    mLastModificationLabel = new QLabel(this);
    mLastModificationLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mLastModificationLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mLastModificationLabel->setLineWidth(2);
    mLastModificationLabel->setMidLineWidth(2);
    addPermanentWidget(mLastModificationLabel, 1);
}

StatusBar::~StatusBar()
{
}

void StatusBar::changeLineCountMessage(const QString &lineCountMessage)
{
    mLineCountLabel->setText(lineCountMessage);
}

void StatusBar::changeLastModification(const QTime &lastModification)
{
    // lastModificationLabel->setText(i18n("Last updated: %1.",
    // KLocale::global()->formatTime(lastModification, true, false) ));
    mLastModificationLabel->setText(
        i18n("Last updated: %1.", QLocale().toString(lastModification, QStringLiteral("hh:mm:ss"))));
}

void StatusBar::changeMessage(const QString &message)
{
    // messageLabel->setText(message);
    // messageList->insertItem(0, i18n("%1: %2", KLocale::global()->formatTime(QTime::currentTime(), true,
    // false), message));
    mMessageList->insertItem(
        0, i18n("%1: %2", QLocale().toString(QTime::currentTime(), QStringLiteral("hh:mm:ss")), message));

    // 100 log history message max.
    if (mMessageList->count() > 100) {
        mMessageList->removeItem(mMessageList->count() - 1);
    }
}

void StatusBar::selectLastHistory()
{
    mMessageList->setCurrentIndex(0);
}

void StatusBar::toggleHistory()
{
    logDebug() << "Toggling History...";
    mMessageList->showPopup();
}
