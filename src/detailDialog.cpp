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

#include "detailDialog.h"

// Qt includes

#include <KGuiItem>
#include <KLocalizedString>
#include <KStandardGuiItem>
#include <QIcon>

#include "logLine.h"
#include "logViewWidget.h"
#include "logViewWidgetItem.h"

#include "logging.h"

DetailDialog::DetailDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    previous->setText(i18n("&Previous"));
    previous->setIcon(QIcon::fromTheme(QStringLiteral("arrow-up")));
    connect(previous, &QAbstractButton::clicked, this, &DetailDialog::previousItem);

    next->setText(i18n("&Next"));
    next->setIcon(QIcon::fromTheme(QStringLiteral("arrow-down")));
    connect(next, &QAbstractButton::clicked, this, &DetailDialog::nextItem);

    KGuiItem::assign(closeButton, KStandardGuiItem::close());
    connect(closeButton, &QAbstractButton::clicked, this, &QWidget::close);
}

DetailDialog::~DetailDialog()
{
}

void DetailDialog::selectionChanged(LogViewWidget *logViewWidget)
{
    mLogViewWidget = logViewWidget;

    updateDetails();
}

// TODO Try to find a method that reload (an resize) correctly the content of the detail dialog
void DetailDialog::updateDetails()
{
    // logDebug() << "Updating Detail Dialog...";

    // Get the current-last item selected
    LogViewWidgetItem *item = mLogViewWidget->lastSelectedItem();
    if (!item) {
        logDebug() << "No item found.";
        return;
    }

    icon->setPixmap(item->logLine()->logLevel()->icon().pixmap(style()->pixelMetric(QStyle::PM_LargeIconSize)));

    header->setText(item->logLine()->formattedText());

    message->setText(item->logLine()->logItems().last());

    if (!mLogViewWidget->topLevelItem(mLogViewWidget->indexOfTopLevelItem(item) - 1)) {
        previous->setEnabled(false);
    } else {
        previous->setEnabled(true);
    }

    if (!mLogViewWidget->topLevelItem(mLogViewWidget->indexOfTopLevelItem(item) + 1)) {
        next->setEnabled(false);
    } else {
        next->setEnabled(true);
    }

    /*
    header->adjustSize();
    adjustSize();
    */
}

void DetailDialog::moveToItem(int direction)
{
    if (direction < 0) {
        logDebug() << "Go to previous item...";
    } else {
        logDebug() << "Go to next item...";
    }

    // Get the current-last item selected
    LogViewWidgetItem *item = mLogViewWidget->lastSelectedItem();
    if (!item) {
        logDebug() << "No item found.";
        return;
    }

    QTreeWidgetItem *destinationItem = mLogViewWidget->topLevelItem(mLogViewWidget->indexOfTopLevelItem(item) + direction);
    if (!destinationItem) {
        if (direction < 0) {
            logDebug() << "No previous item found.";
        } else {
            logDebug() << "No next item found.";
        }
        return;
    }

    item->setSelected(false);
    destinationItem->setSelected(true);
    mLogViewWidget->scrollToItem(destinationItem);

    updateDetails();
}

void DetailDialog::previousItem()
{
    moveToItem(-1);
}

void DetailDialog::nextItem()
{
    moveToItem(1);
}
