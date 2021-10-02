/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include "analyzer.h"
#include "globals.h"

class LogLine;
class LogViewWidget;

class LogViewModel : public QObject
{
    Q_OBJECT

public:
    explicit LogViewModel(LogViewWidget *logViewWidget);

    ~LogViewModel() override;

    /**
     * Clear the model
     */
    void clear();

    bool insertNewLogLine(LogLine *line);

    int itemCount() const;
    bool isEmpty() const;

    bool isProcessingMultipleInsertions() const;

    void startingMultipleInsertions();
    void endingMultipleInsertions(Analyzer::ReadingMode readingMode, int insertedLogLineCount);

    QList<LogLine *> logLines() const;

Q_SIGNALS:
    void processingMultipleInsertions(bool currentlyInserting);

private:
    /**
     * Prevent crossed multiple insertions between each LogFileReaders
     */
    bool lockMultipleInsertions();

    bool logLineAlreadyExists(LogLine *line) const;

    bool isNewer(LogLine *line) const;

    /**
     * Remove the oldest line
     */
    void removeOldestLogLine();

    /**
     * Insert this line
     */
    void insert(LogLine *line);

    /**
     * Remove recent status on previously new log lines
     */
    void removeRecentStatusOfLogLines();

    LogViewWidget *mLogViewWidget = nullptr;

    LogViewWidgetItem *mOldestItem = nullptr;

    int mConcurrentMultipleInsertions = 0;
};

