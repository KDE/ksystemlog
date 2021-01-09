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

LogViewModel::LogViewModel(LogViewWidget *logViewWidget)
    : QObject(logViewWidget)
{
    mLogViewWidget = logViewWidget;
}

LogViewModel::~LogViewModel()
{
}

bool LogViewModel::logLineAlreadyExists(LogLine *line) const
{
    LogViewWidgetItem *item = mLogViewWidget->findItem(line);
    if (item != nullptr) {
        return true;
    }

    return false;
}

QList<LogLine *> LogViewModel::logLines() const
{
    return mLogViewWidget->logLines();
}

int LogViewModel::itemCount() const
{
    return mLogViewWidget->itemCount();
}

bool LogViewModel::isEmpty() const
{
    if (mLogViewWidget->itemCount() == 0) {
        return true;
    }

    return false;
}

void LogViewModel::removeRecentStatusOfLogLines()
{
    // The older lines are no longer recent
    foreach (LogViewWidgetItem *item, mLogViewWidget->items()) {
        item->logLine()->setRecent(false);
    }
}

void LogViewModel::startingMultipleInsertions()
{
    bool hasLocked = false;

    // Check the lock before adding this as locker
    if (lockMultipleInsertions()) {
        hasLocked = true;
    }

    // Add a lock
    mConcurrentMultipleInsertions++;

    if (hasLocked) {
        logDebug() << "Starting multiple insertions...";

        emit(processingMultipleInsertions(true));

        mLogViewWidget->setUpdatesEnabled(false);

        // Remove all recent states of previous log lines
        removeRecentStatusOfLogLines();
    }
}

void LogViewModel::endingMultipleInsertions(Analyzer::ReadingMode readingMode, int insertedLogLineCount)
{
    // Remove a lock
    mConcurrentMultipleInsertions--;

    if (lockMultipleInsertions()) {
        logDebug() << "Ending multiple insertions...";

        // Scroll to the newest item if some lines have been added
        if (insertedLogLineCount > 0) {
            mLogViewWidget->scrollToNewestItem();
        }

        if (readingMode == Analyzer::FullRead) {
            mLogViewWidget->resizeColumns();
        }

        logDebug() << "Enabling log view widget refresh...";
        mLogViewWidget->setUpdatesEnabled(true);

        emit(processingMultipleInsertions(false));
    }
}

bool LogViewModel::lockMultipleInsertions()
{
    if (mConcurrentMultipleInsertions == 0) {
        return true;
    }

    // Debug messages
    if (mConcurrentMultipleInsertions > 0) {
        logDebug() << "Existing multiple insertions request is still active";
    } else if (mConcurrentMultipleInsertions < 0) {
        logCritical() << "Existing multiple insertions forgot to call this method";
    }

    return false;
}

bool LogViewModel::isProcessingMultipleInsertions() const
{
    if (mConcurrentMultipleInsertions == 0) {
        return false;
    } else {
        return true;
    }
}

void LogViewModel::clear()
{
    mLogViewWidget->clear();

    // Reinit Oldest item
    mOldestItem = nullptr;
}

bool LogViewModel::isNewer(LogLine *newLine) const
{
    // No element in the list in this case
    if (mOldestItem == nullptr) {
        return true;
    }

    if (newLine->isNewerThan(*(mOldestItem->logLine()))) {
        return true;
    }

    return false;
}

void LogViewModel::removeOldestLogLine()
{
    // logDebug() << "Removing oldest log line";

    if (isEmpty()) {
        return;
    }

    if (!mOldestItem) {
        logWarning() << "Oldest item is null";
        return;
    }

    // Remove the oldest item from the list
    mLogViewWidget->takeTopLevelItem(mLogViewWidget->indexOfTopLevelItem(mOldestItem));

    delete mOldestItem;
    mOldestItem = nullptr;

    // Find the next oldest item
    foreach (LogViewWidgetItem *item, mLogViewWidget->items()) {
        if (mOldestItem == nullptr) {
            mOldestItem = item;
            continue;
        }

        if (mOldestItem->logLine()->isNewerThan(*(item->logLine()))) {
            mOldestItem = item;
        }
    }
}

void LogViewModel::insert(LogLine *line)
{
    // The item is automatically added to the LogViewWidget
    auto *item = new LogViewWidgetItem(mLogViewWidget, line);

    // Update the oldest item
    if (mOldestItem == nullptr) {
        mOldestItem = item;
    } else if (mOldestItem->logLine()->isNewerThan(*line)) {
        mOldestItem = item;
    }
}

bool LogViewModel::insertNewLogLine(LogLine *line)
{
    // If the Delete Duplicated Line option is checked
    if (KSystemLogConfig::deleteDuplicatedLines()) {
        if (logLineAlreadyExists(line)) {
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
    else if (isNewer(line)) {
        removeOldestLogLine();
        insert(line);

        return true;
    }

    // logDebug() << "Do not insert an old line : " << line->logItems();

    return false;
}
