/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logViewModel.h"

#include "logLine.h"
#include "view.h"

#include "logModeItemBuilder.h"
#include "logViewWidget.h"
#include "logViewWidgetItem.h"

#include "ksystemlog_debug.h"

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
    if (item) {
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
    const auto items = mLogViewWidget->items();
    for (LogViewWidgetItem *item : items) {
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
        qCDebug(KSYSTEMLOG) << "Starting multiple insertions...";

        Q_EMIT processingMultipleInsertions(true);

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
        qCDebug(KSYSTEMLOG) << "Ending multiple insertions...";

        // Scroll to the newest item if some lines have been added
        if (insertedLogLineCount > 0) {
            mLogViewWidget->scrollToNewestItem();
        }

        if (readingMode == Analyzer::FullRead) {
            mLogViewWidget->resizeColumns();
        }

        qCDebug(KSYSTEMLOG) << "Enabling log view widget refresh...";
        mLogViewWidget->setUpdatesEnabled(true);

        Q_EMIT processingMultipleInsertions(false);
    }
}

bool LogViewModel::lockMultipleInsertions()
{
    if (mConcurrentMultipleInsertions == 0) {
        return true;
    } else if (mConcurrentMultipleInsertions > 0) {
        qCDebug(KSYSTEMLOG) << "Existing multiple insertions request is still active";
    } else {
        qCCritical(KSYSTEMLOG) << "Existing multiple insertions forgot to call this method";
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
    if (!mOldestItem) {
        return true;
    }

    if (newLine->isNewerThan(*(mOldestItem->logLine()))) {
        return true;
    }

    return false;
}

void LogViewModel::removeOldestLogLine()
{
    // qCDebug(KSYSTEMLOG) << "Removing oldest log line";

    if (isEmpty()) {
        return;
    }

    if (!mOldestItem) {
        qCWarning(KSYSTEMLOG) << "Oldest item is null";
        return;
    }

    // Remove the oldest item from the list
    mLogViewWidget->takeTopLevelItem(mLogViewWidget->indexOfTopLevelItem(mOldestItem));

    delete mOldestItem;
    mOldestItem = nullptr;

    // Find the next oldest item
    const auto items{mLogViewWidget->items()};
    for (LogViewWidgetItem *item : items) {
        if (!mOldestItem) {
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
    auto item = new LogViewWidgetItem(mLogViewWidget, line);

    // Update the oldest item
    if (!mOldestItem) {
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

    // qCDebug(KSYSTEMLOG) << "Do not insert an old line : " << line->logItems();

    return false;
}

#include "moc_logViewModel.cpp"
