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

#include "logViewModel.h"

#include "view.h"
#include "logLine.h"

#include "logViewWidgetItem.h"
#include "logViewWidget.h"
#include "logModeItemBuilder.h"

#include "logging.h"

#include "ksystemlogConfig.h"

class LogViewModelPrivate
{
public:
    LogViewWidget *mLogViewWidget = nullptr;

    LogViewWidgetItem *mOldestItem = nullptr;

    int mConcurrentMultipleInsertions = 0;
};

LogViewModel::LogViewModel(LogViewWidget *logViewWidget)
    : QObject(logViewWidget)
    , d(new LogViewModelPrivate())
{
    d->mLogViewWidget = logViewWidget;
}

LogViewModel::~LogViewModel()
{
    delete d;
}

bool LogViewModel::logLineAlreadyExists(LogLine *line) const
{
    LogViewWidgetItem *item = d->mLogViewWidget->findItem(line);
    if (item != nullptr)
        return true;

    return false;
}

QList<LogLine *> LogViewModel::logLines() const
{
    return d->mLogViewWidget->logLines();
}

int LogViewModel::itemCount() const
{
    return d->mLogViewWidget->itemCount();
}

bool LogViewModel::isEmpty() const
{
    if (d->mLogViewWidget->itemCount() == 0)
        return true;

    return false;
}

void LogViewModel::removeRecentStatusOfLogLines()
{
    // The older lines are no longer recent
    foreach (LogViewWidgetItem *item, d->mLogViewWidget->items()) {
        item->logLine()->setRecent(false);
    }
}

void LogViewModel::startingMultipleInsertions()
{
    bool hasLocked = false;

    // Check the lock before adding this as locker
    if (lockMultipleInsertions() == true) {
        hasLocked = true;
    }

    // Add a lock
    d->mConcurrentMultipleInsertions++;

    if (hasLocked == true) {
        logDebug() << "Starting multiple insertions...";

        emit(processingMultipleInsertions(true));

        d->mLogViewWidget->setUpdatesEnabled(false);

        // Remove all recent states of previous log lines
        removeRecentStatusOfLogLines();
    }
}

void LogViewModel::endingMultipleInsertions(Analyzer::ReadingMode readingMode, int insertedLogLineCount)
{
    // Remove a lock
    d->mConcurrentMultipleInsertions--;

    if (lockMultipleInsertions() == true) {
        logDebug() << "Ending multiple insertions...";

        // Scroll to the newest item if some lines have been added
        if (insertedLogLineCount > 0) {
            d->mLogViewWidget->scrollToNewestItem();
        }

        if (readingMode == Analyzer::FullRead) {
            d->mLogViewWidget->resizeColumns();
        }

        logDebug() << "Enabling log view widget refresh...";
        d->mLogViewWidget->setUpdatesEnabled(true);

        emit(processingMultipleInsertions(false));
    }
}

bool LogViewModel::lockMultipleInsertions()
{
    if (d->mConcurrentMultipleInsertions == 0) {
        return true;
    }

    // Debug messages
    if (d->mConcurrentMultipleInsertions > 0) {
        logDebug() << "Existing multiple insertions request is still active";
    } else if (d->mConcurrentMultipleInsertions < 0) {
        logCritical() << "Existing multiple insertions forgot to call this method";
    }

    return false;
}

bool LogViewModel::isProcessingMultipleInsertions() const
{
    if (d->mConcurrentMultipleInsertions == 0)
        return false;
    else
        return true;
}

void LogViewModel::clear()
{
    d->mLogViewWidget->clear();

    // Reinit Oldest item
    d->mOldestItem = nullptr;
}

bool LogViewModel::isNewer(LogLine *newLine) const
{
    // No element in the list in this case
    if (d->mOldestItem == nullptr)
        return true;

    if (newLine->isNewerThan(*(d->mOldestItem->logLine())))
        return true;

    return false;
}

void LogViewModel::removeOldestLogLine()
{
    // logDebug() << "Removing oldest log line";

    if (isEmpty() == true) {
        return;
    }

    if (d->mOldestItem == nullptr) {
        logWarning() << "Oldest item is null";
        return;
    }

    // Remove the oldest item from the list
    d->mLogViewWidget->takeTopLevelItem(d->mLogViewWidget->indexOfTopLevelItem(d->mOldestItem));

    delete d->mOldestItem;
    d->mOldestItem = nullptr;

    // Find the next oldest item
    foreach (LogViewWidgetItem *item, d->mLogViewWidget->items()) {
        if (d->mOldestItem == nullptr) {
            d->mOldestItem = item;
            continue;
        }

        if (d->mOldestItem->logLine()->isNewerThan(*(item->logLine()))) {
            d->mOldestItem = item;
        }
    }
}

void LogViewModel::insert(LogLine *line)
{
    // The item is automatically added to the LogViewWidget
    LogViewWidgetItem *item = new LogViewWidgetItem(d->mLogViewWidget, line);

    // Update the oldest item
    if (d->mOldestItem == nullptr) {
        d->mOldestItem = item;
    } else if (d->mOldestItem->logLine()->isNewerThan(*line)) {
        d->mOldestItem = item;
    }
}

bool LogViewModel::insertNewLogLine(LogLine *line)
{
    // If the Delete Duplicated Line option is checked
    if (KSystemLogConfig::deleteDuplicatedLines() == true) {
        if (logLineAlreadyExists(line) == true) {
            delete line;
            return false;
        }
    }

    // If there is still space in the buffer
    if (itemCount() < KSystemLogConfig::maxLines()) {
        insert(line);

        return true;
    }
    // If the line is newer, it can be inserted
    else if (isNewer(line) == true) {
        removeOldestLogLine();
        insert(line);

        return true;
    }

    // logDebug() << "Do not insert an old line : " << line->logItems();

    return false;
}
