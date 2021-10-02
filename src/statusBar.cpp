/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "statusBar.h"

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
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
    connect(mMessageList, &QComboBox::currentIndexChanged, this, &StatusBar::selectLastHistory);
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

void StatusBar::changeLastModification(QTime lastModification)
{
    // lastModificationLabel->setText(i18n("Last updated: %1.",
    // KLocale::global()->formatTime(lastModification, true, false) ));
    mLastModificationLabel->setText(i18n("Last updated: %1.", QLocale().toString(lastModification, QStringLiteral("hh:mm:ss"))));
}

void StatusBar::changeMessage(const QString &message)
{
    // messageLabel->setText(message);
    // messageList->insertItem(0, i18n("%1: %2", KLocale::global()->formatTime(QTime::currentTime(), true,
    // false), message));
    mMessageList->insertItem(0, i18n("%1: %2", QLocale().toString(QTime::currentTime(), QStringLiteral("hh:mm:ss")), message));

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
