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

#ifndef ANALYZER_H
#define ANALYZER_H

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

    virtual void setLogFiles(const QList<LogFile> &logFiles) = 0;

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

    bool parsingPaused;
    LogViewModel *logViewModel = nullptr;
    LogMode *const logMode;
    QMutex insertionLocking;
    long logLineInternalIdGenerator = 0;
};

#endif
