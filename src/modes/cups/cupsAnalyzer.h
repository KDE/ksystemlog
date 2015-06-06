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

#ifndef _CUPS_ANALYZER_H_
#define _CUPS_ANALYZER_H_

#include <KLocalizedString>

#include "analyzer.h"

#include "logging.h"

#include "localLogFileReader.h"
#include "cupsLogMode.h"
#include "parsingHelper.h"

#define DEBUG2_LOG_LEVEL_ICON "source"

class CupsAnalyzer : public Analyzer
{
    Q_OBJECT

public:
    explicit CupsAnalyzer(LogMode *logMode)
        : Analyzer(logMode)
    {
        initializeTypeLevels();
    }

    virtual ~CupsAnalyzer() {}

    LogViewColumns initColumns()
    {
        LogViewColumns columns;
        columns.addColumn(LogViewColumn(i18n("Date"), true, false));
        columns.addColumn(LogViewColumn(i18n("Message"), true, false));

        return columns;
    }

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) { return new LocalLogFileReader(logFile); }

    Analyzer::LogFileSortMode logFileSortMode() { return Analyzer::AscendingSortedLogFile; }

    /*
     * Also sees :
     * http://www.cups.org/documentation.php/ref-error_log.html
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
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile)
    {
        QString line(logLine);

        QChar level = logLine[0];

        QDateTime dateTime = ParsingHelper::instance()->parseHttpDateTime(logLine.mid(3, 26));

        QString message = line.remove(0, 31);

        QStringList list;
        list.append(message);

        return new LogLine(logLineInternalIdGenerator++, dateTime, list, originalLogFile.url().path(),
                           findLogLevel(level), logMode);
    }

private:
    QMap<QChar, LogLevel *> mapTypeLevels;

    void initializeTypeLevels()
    {
        mapTypeLevels[QLatin1Char('d')]
            = new LogLevel(20, i18n("debug 2"), QLatin1String(DEBUG2_LOG_LEVEL_ICON), QColor(169, 189, 165));
        mapTypeLevels[QLatin1Char('D')] = Globals::instance()->debugLogLevel();
        mapTypeLevels[QLatin1Char('I')] = Globals::instance()->informationLogLevel();
        mapTypeLevels[QLatin1Char('N')] = Globals::instance()->noticeLogLevel();
        mapTypeLevels[QLatin1Char('W')] = Globals::instance()->warningLogLevel();
        mapTypeLevels[QLatin1Char('E')] = Globals::instance()->errorLogLevel();
        mapTypeLevels[QLatin1Char('C')] = Globals::instance()->criticalLogLevel();
        mapTypeLevels[QLatin1Char('A')] = Globals::instance()->alertLogLevel();
        mapTypeLevels[QLatin1Char('X')] = Globals::instance()->emergencyLogLevel();
        mapTypeLevels[QLatin1Char(' ')] = Globals::instance()->noLogLevel();
    }

    LogLevel *findLogLevel(const QChar &type)
    {
        QMap<QChar, LogLevel *>::iterator it;

        it = mapTypeLevels.find(type);
        if (it != mapTypeLevels.end()) {
            return (*it);
        } else {
            logCritical() << i18n(
                "New Log Level detected: Please send this log file to the KSystemLog developer to add it.");
            return (Globals::instance()->noLogLevel());
        }
    }
};

#endif // _CUPS_ANALYZER_H_
