/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include "fileAnalyzer.h"

#include "logging.h"

#include "cupsLogMode.h"
#include "localLogFileReader.h"
#include "parsingHelper.h"

#define DEBUG2_LOG_LEVEL_ICON "source"

class CupsAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit CupsAnalyzer(LogMode *logMode);

    ~CupsAnalyzer() override;

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    /*
     * Also sees :
     * https://www.cups.org/doc/man-cupsd-logs.html
     * level date-time message
     *
     * Levels :
     * A - Alert message (LogLevel alert)
     * C - Critical error message (LogLevel crit)
     * D - Debugging message (LogLevel debug)
     * d - Detailed debugging message (LogLevel debug2)
     * E - Normal error message (LogLevel error)
     * I - Informational message (LogLevel info)
     * N - Notice message (LogLevel notice)
     * W - Warning message (LogLevel warn)
     * X - Emergency error message (LogLevel emerg)
     *
     * Log line examples :
     * I [15/Feb/2004:01:29:32 +0100] LoadPPDs: No new or changed PPDs...
     * E [15/Feb/2004:01:43:15 +0100] Scheduler shutting down due to SIGTERM.
     *
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

private:
    QMap<QChar, LogLevel *> mMapTypeLevels;

    void initializeTypeLevels();

    LogLevel *findLogLevel(QChar type);
};

