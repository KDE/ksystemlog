/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "detailDialog.h"

// Qt includes

#include <KGuiItem>
#include <KLocalizedString>
#include <KStandardGuiItem>
#include <QIcon>

#include "logLine.h"
#include "logViewWidget.h"
#include "logViewWidgetItem.h"

#include "ksystemlog_debug.h"

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
    // qCDebug(KSYSTEMLOG) << "Updating Detail Dialog...";

    // Get the current-last item selected
    LogViewWidgetItem *item = mLogViewWidget->lastSelectedItem();
    if (!item) {
        qCDebug(KSYSTEMLOG) << "No item found.";
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
        qCDebug(KSYSTEMLOG) << "Go to previous item...";
    } else {
        qCDebug(KSYSTEMLOG) << "Go to next item...";
    }

    // Get the current-last item selected
    LogViewWidgetItem *item = mLogViewWidget->lastSelectedItem();
    if (!item) {
        qCDebug(KSYSTEMLOG) << "No item found.";
        return;
    }

    QTreeWidgetItem *destinationItem = mLogViewWidget->topLevelItem(mLogViewWidget->indexOfTopLevelItem(item) + direction);
    if (!destinationItem) {
        if (direction < 0) {
            qCDebug(KSYSTEMLOG) << "No previous item found.";
        } else {
            qCDebug(KSYSTEMLOG) << "No next item found.";
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

#include "moc_detailDialog.cpp"
