/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QList>
#include <QMutex>
#include <QString>

#include "globals.h"

#include "logLine.h"

#include "logFile.h"
#include "logViewColumn.h"
#include "logViewColumns.h"

class LogViewModel;
class LogMode;

class Analyzer : public QObject
{
    Q_OBJECT

public:
    enum ReadingMode { UpdatingRead, FullRead };
    Q_DECLARE_FLAGS(ReadingModes, ReadingMode)

    enum LogFileSortMode { AscendingSortedLogFile, FilteredLogFile, UnsortedLogFile };
    Q_DECLARE_FLAGS(LogFileSortModes, LogFileSortMode)

    explicit Analyzer(LogMode *mode);

    ~Analyzer() override;

    virtual void watchLogFiles(bool enabled) = 0;

    virtual void setLogFiles(const QVector<LogFile> &logFiles) = 0;

    virtual LogViewColumns initColumns() = 0;

    void setLogViewModel(LogViewModel *logViewModel);

    bool isParsingPaused() const;

public Q_SLOTS:
    void setParsingPaused(bool paused);

Q_SIGNALS:
    void statusBarChanged(const QString &message);
    void errorOccured(const QString &title, const QString &message);
    void statusChanged(const QString &status);

    void openingProgressed();

    void logUpdated(int lineTotal);

    void readFileStarted(const LogMode &logMode, const LogFile &logFile, int fileIndex, int fileCount);
    void readEnded();

protected:
    void informOpeningProgress(int currentPosition, int total);

    bool mParsingPaused = false;
    LogViewModel *mLogViewModel = nullptr;
    LogMode *const mLogMode;
    QRecursiveMutex mInsertionLocking;
    long mLogLineInternalIdGenerator = 0;
};

