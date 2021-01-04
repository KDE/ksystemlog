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

namespace KSystemLog
{
class StatusBarPrivate
{
public:
    QLabel *lineCountLabel = nullptr;

    // KSqueezedTextLabel* messageLabel;

    KComboBox *messageList = nullptr;

    QLabel *lastModificationLabel = nullptr;

    QPushButton *toggleHistory = nullptr;
};

StatusBar::StatusBar(QWidget *parent)
    : QStatusBar(parent)
    , d(new StatusBarPrivate())
{
    d->lineCountLabel = new QLabel(this);
    d->lineCountLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    d->lineCountLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    d->lineCountLabel->setLineWidth(2);
    d->lineCountLabel->setMidLineWidth(2);
    addPermanentWidget(d->lineCountLabel, 1);

    /*
    d->toggleHistory = new QPushButton(this);
    d->toggleHistory->setIcon(QIcon::fromTheme( QLatin1String( "view-history" )));
    d->toggleHistory->setFlat(true);
    addPermanentWidget(d->toggleHistory, 0);

    connect(d->toggleHistory, &QPushButton::clicked, this, &StatusBar::toggleHistory);
    */

    /*
        d->messageLabel = new KSqueezedTextLabel("", this);
        d->messageLabel->setAlignment(Qt::AlignLeft);
        d->messageLabel->setTextElideMode(Qt::ElideRight);
        addPermanentWidget(d->messageLabel, 4);
    */
    d->messageList = new KComboBox(this);
    d->messageList->setInsertPolicy(QComboBox::InsertAtTop);
    d->messageList->setMaxVisibleItems(5);
    connect(d->messageList, QOverload<int>::of(&KComboBox::currentIndexChanged), this, &StatusBar::selectLastHistory);
    /*
        //TODO Define a specifical palette (and make it works !)
        QPalette palette(d->messageList->palette());
        palette.setColor(QPalette::HighlightedText, Qt::red); //palette.color(QPalette::Base)
        palette.setColor(QPalette::Base, Qt::red); //palette.color(QPalette::Base)
        palette.setColor(QPalette::Text, QColor(212, 140, 95)); //palette.color(QPalette::Base)
        d->messageList->setPalette(palette);
        //d->messageList->repaint();
    */
    addPermanentWidget(d->messageList, 4);

    d->lastModificationLabel = new QLabel(this);
    d->lastModificationLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    d->lastModificationLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    d->lastModificationLabel->setLineWidth(2);
    d->lastModificationLabel->setMidLineWidth(2);
    addPermanentWidget(d->lastModificationLabel, 1);
}

StatusBar::~StatusBar()
{
    // QLabels are automatically deleted by Qt
    delete d;
}

void StatusBar::changeLineCountMessage(const QString &lineCountMessage)
{
    d->lineCountLabel->setText(lineCountMessage);
}

void StatusBar::changeLastModification(const QTime &lastModification)
{
    // d->lastModificationLabel->setText(i18n("Last updated: %1.",
    // KLocale::global()->formatTime(lastModification, true, false) ));
    d->lastModificationLabel->setText(
        i18n("Last updated: %1.", QLocale().toString(lastModification, QStringLiteral("hh:mm:ss"))));
}

void StatusBar::changeMessage(const QString &message)
{
    // d->messageLabel->setText(message);
    // d->messageList->insertItem(0, i18n("%1: %2", KLocale::global()->formatTime(QTime::currentTime(), true,
    // false), message));
    d->messageList->insertItem(
        0, i18n("%1: %2", QLocale().toString(QTime::currentTime(), QStringLiteral("hh:mm:ss")), message));

    // 100 log history message max.
    if (d->messageList->count() > 100) {
        d->messageList->removeItem(d->messageList->count() - 1);
    }
}

void StatusBar::selectLastHistory()
{
    d->messageList->setCurrentIndex(0);
}

void StatusBar::toggleHistory()
{
    logDebug() << "Toggling History...";
    d->messageList->showPopup();
}
}
