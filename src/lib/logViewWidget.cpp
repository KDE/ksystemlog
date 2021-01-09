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

#include "logViewWidget.h"

#include <QAction>
#include <QHeaderView>

#include <QIcon>
#include <KLocalizedString>

#include "logging.h"

#include "logViewColumn.h"

#include "logViewWidgetItem.h"
#include "logLine.h"
#include "logViewModel.h"

#include "ksystemlogConfig.h"

LogViewWidget::LogViewWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    // TODO Add this setWhatsThis() to all columns each time they change
    // setWhatThis(i18n("<p>This is the main view of KSystemLog. It displays the last lines of the selected
    // log. Please see the documentation to discovers the meaning of each icons and existing log.</p><p>Log
    // lines in <b>bold</b> are the last added to the list.</p>"));

    const QStringList headerLabels {i18n("Date"), i18n("Message")};

    mLogViewModel = new LogViewModel(this);
    mHeadersTogglingActions = new QActionGroup(this);
    mHeadersTogglingActions->setExclusive(false);
    connect(mHeadersTogglingActions, &QActionGroup::triggered, this, &LogViewWidget::toggleHeader);

    setHeaderLabels(headerLabels);

    // Header
    header()->setContextMenuPolicy(Qt::ActionsContextMenu);
    // header()->setMovable(true);
    header()->setSectionsMovable(true);

    setSortingEnabled(true);
    sortItems(0, Qt::AscendingOrder);

    setAnimated(true);

    setRootIsDecorated(false);

    setAllColumnsShowFocus(true);

    setAlternatingRowColors(true);

    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setContextMenuPolicy(Qt::ActionsContextMenu);
}

LogViewWidget::~LogViewWidget()
{
    delete mLogViewModel;
}

void LogViewWidget::setColumns(const LogViewColumns &columns)
{
    logDebug() << "Updating columns using " << columns << "...";

    // First, delete all current columns
    setColumnCount(0);

    setHeaderLabels(columns.toStringList());

    sortItems(0, Qt::AscendingOrder);

    // Remove previous header actions
    QListIterator<QAction *> it(mHeadersTogglingActions->actions());
    it.toBack();
    while (it.hasPrevious()) {
        QAction *action = it.previous();

        header()->removeAction(action);
        mHeadersTogglingActions->removeAction(action);

        delete action;
    }

    // Add new actions
    int columnIndex = 0;

    foreach (const LogViewColumn &column, columns.columns()) {
        auto *action = new QAction(this);
        action->setText(column.columnName());
        // helloAction->setIcon(QIcon::fromTheme( QLatin1String( "media-playback-start" )));
        // helloAction->setShortcut(Qt::CTRL | Qt::Key_M);
        action->setCheckable(true);
        action->setChecked(true);
        action->setToolTip(i18n("Display/Hide the '%1' column", column.columnName()));
        action->setData(QVariant(columnIndex));

        mHeadersTogglingActions->addAction(action);

        ++columnIndex;
    }

    header()->addActions(mHeadersTogglingActions->actions());

    Q_EMIT columnsChanged(columns);

    logDebug() << "Log View Widget updated...";
}

void LogViewWidget::resizeColumns()
{
    // Resize all columns except the last one (which always take the last available space)
    for (int i = 0; i < columnCount() - 1; ++i) {
        resizeColumnToContents(i);
    }
}

void LogViewWidget::selectAll()
{
    if (notHiddenItemCount() > 0) {
        QTreeWidget::selectAll();
    }
}

int LogViewWidget::itemCount() const
{
    return topLevelItemCount();
}

QList<LogLine *> LogViewWidget::logLines()
{
    QList<LogLine *> logLines;

    QTreeWidgetItemIterator it(this);
    while (*it != nullptr) {
        auto *item = static_cast<LogViewWidgetItem *>(*it);
        logLines.append(item->logLine());
        ++it;
    }

    return logLines;
}

LogViewWidgetItem *LogViewWidget::findNewestItem()
{
    LogViewWidgetItem *newestItem = nullptr;

    QTreeWidgetItemIterator it(this);
    while (*it != nullptr) {
        auto *item = static_cast<LogViewWidgetItem *>(*it);
        if (newestItem == nullptr || newestItem->logLine()->isOlderThan(*(item->logLine()))) {
            newestItem = item;
        }

        ++it;
    }

    return newestItem;
}

LogViewWidgetItem *LogViewWidget::findItem(LogLine *searchedLogLine)
{
    QTreeWidgetItemIterator it(this);
    while (*it != nullptr) {
        auto *item = static_cast<LogViewWidgetItem *>(*it);
        if (item->logLine()->equals(*searchedLogLine)) {
            return item;
        }

        ++it;
    }

    return nullptr;
}

QList<LogViewWidgetItem *> LogViewWidget::items()
{
    QList<LogViewWidgetItem *> items;

    QTreeWidgetItemIterator it(this);
    while (*it != nullptr) {
        items.append(static_cast<LogViewWidgetItem *>(*it));
        ++it;
    }

    return items;
}

LogViewModel *LogViewWidget::model() const
{
    return mLogViewModel;
}

bool LogViewWidget::hasItemsSelected()
{
    return !selectedItems().isEmpty();
}

LogViewWidgetItem *LogViewWidget::firstSelectedItem()
{
    QTreeWidgetItemIterator it(this, QTreeWidgetItemIterator::Selected);

    // Returns the first selected item or NULL is there is no item selected
    return static_cast<LogViewWidgetItem *>(*it);
}

LogViewWidgetItem *LogViewWidget::lastSelectedItem()
{
    QTreeWidgetItemIterator it(this, QTreeWidgetItemIterator::Selected);

    QTreeWidgetItem *item = nullptr;
    while (*it) {
        item = (*it);

        ++it;
    }

    // Returns the last selected item or NULL is there is no item selected
    return static_cast<LogViewWidgetItem *>(item);
}

void LogViewWidget::expandAll()
{
    QTreeWidgetItemIterator it(this);
    while (*it != nullptr) {
        expandItem(*it);
        ++it;
    }
}

void LogViewWidget::collapseAll()
{
    QTreeWidgetItemIterator it(this);
    while (*it != nullptr) {
        collapseItem(*it);
        ++it;
    }
}

void LogViewWidget::toggleToolTip(bool enabled)
{
    logDebug() << "Toggle tool tip " << enabled;

    QTreeWidgetItemIterator it(this);
    while (*it != nullptr) {
        auto *item = static_cast<LogViewWidgetItem *>(*it);
        item->toggleToolTip(enabled);

        ++it;
    }
}

void LogViewWidget::scrollToNewestItem()
{
    logDebug() << "Scrolling to the newest item...";

    // Scroll to last item if requested
    if (KSystemLogConfig::newLinesDisplayed() == true) {
        LogViewWidgetItem *newestItem = findNewestItem();
        if (newestItem != nullptr) {
            scrollToItem(newestItem);
        }
    }
}

int LogViewWidget::notHiddenItemCount()
{
    int count = 0;

    QTreeWidgetItemIterator it(this, QTreeWidgetItemIterator::NotHidden);
    while (*it != nullptr) {
        count++;
        ++it;
    }

    return count;
}

void LogViewWidget::toggleHeader(QAction *action)
{
    logDebug() << "Toggling header";

    int columnIndex = action->data().toInt();
    if (header()->isSectionHidden(columnIndex) == true) {
        header()->setSectionHidden(columnIndex, false);
    } else {
        header()->setSectionHidden(columnIndex, true);
    }
}
