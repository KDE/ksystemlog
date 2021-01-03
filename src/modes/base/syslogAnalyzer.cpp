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

#include "syslogAnalyzer.h"

#include <QStringList>
#include <QDateTime>

#include <KLocalizedString>

#include "globals.h"
#include "logging.h"

#include "localLogFileReader.h"
#include "logLine.h"
#include "logMode.h"
#include "logLevel.h"
#include "logViewWidget.h"

#include "logViewModel.h"

#include "parsingHelper.h"

#include "ksystemlogConfig.h"

SyslogAnalyzer::SyslogAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
}

SyslogAnalyzer::~SyslogAnalyzer()
{
}

LogViewColumns SyslogAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Host"), true, true));
    columns.addColumn(LogViewColumn(i18n("Process"), true, true));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

LogFileReader *SyslogAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode SyslogAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

/**
 * TODO Improve speed of this method (with KRegExp class for example)
 */
LogLine *SyslogAnalyzer::parseMessage(const QString &logLine, const LogFile &originalFile)
{
    // logDebug() << QTime::currentTime() << " : Reading line : " << logLine << " from " <<
    // originalFile.url.path();

    // 15 is the default date size format
    if (logLine.length() < 15) {
        logDebug() << "Too short line";
        return undefinedLogLine(logLine, originalFile);
    }

    const int year = QDate::currentDate().year();

    // Month number
    QString month(logLine.left(3));

    QString line(logLine);

    line.remove(0, 4);
    int monthNum = ParsingHelper::instance()->parseSyslogMonth(month);

    // Day number
    QString day(line.left(2));
    int dayNum = day.toInt();

    line.remove(0, 3);

    // Time
    QString stringTime(line.left(8));
    int h = stringTime.leftRef(2).toInt();
    stringTime.remove(0, 3);
    int m = stringTime.leftRef(2).toInt();
    stringTime.remove(0, 3);
    int s = stringTime.leftRef(2).toInt();
    stringTime.remove(0, 3);

    QDateTime dateTime(QDate(year, monthNum, dayNum), QTime(h, m, s));
    if (dateTime.isValid() == false) {
        logDebug() << "Malformed date and time";
        return undefinedLogLine(logLine, originalFile);
    }

    line.remove(0, 9);

    QString hostname;

    int nextSpace = line.indexOf(QLatin1Char(' '));
    int nextDoubleDot = line.indexOf(QLatin1Char(':'));

    // Normal case or no process name
    if (nextSpace < nextDoubleDot || nextDoubleDot == -1) {
        // Host name
        hostname = line.left(nextSpace);
        line.remove(0, nextSpace + 1);
    }
    // No host name case (very rare)
    else {
        // Host name
        hostname = undefinedHostName();
    }

    // Refresh double dot once the line has been substr'ed
    nextDoubleDot = line.indexOf(QLatin1Char(':'));

    QString process;
    QString message;

    // Process name
    if (nextDoubleDot != -1) {
        process = line.left(nextDoubleDot);

        // If the delete process identifier option is enabled
        if (KSystemLogConfig::deleteProcessIdentifier() == true) {
            int squareBracket = process.indexOf(QLatin1Char('['));

            // If we find a bracket, we remove the useless part
            if (squareBracket != -1) {
                process.truncate(squareBracket);
            }
        }
        line.remove(0, nextDoubleDot + 1);

        message = line.remove(0, 1);
    }
    // If we can't find any ':' character, it means that this line is a
    // internal message of syslogd
    else {
        if (line.contains(QLatin1String("last message repeated"))
            || line.contains(QLatin1String("-- MARK --"))) {
            process = QStringLiteral("syslog");
        } else {
            process = undefinedProcess();
        }

        message = line;
    }

    QStringList list;
    list.append(hostname);
    list.append(process);
    list.append(message);

    return new LogLine(logLineInternalIdGenerator++, dateTime, list, originalFile.url().toLocalFile(),
                       originalFile.defaultLogLevel(), logMode);
}

inline LogLine *SyslogAnalyzer::undefinedLogLine(const QString &message, const LogFile &originalFile)
{
    QStringList items;
    items << undefinedHostName() << undefinedProcess() << message;
    return new LogLine(logLineInternalIdGenerator++, QDateTime::currentDateTime(), items,
                       originalFile.url().toLocalFile(), originalFile.defaultLogLevel(), logMode);
}

inline QString SyslogAnalyzer::undefinedHostName()
{
    // i18nc("Undefined host name", "undefined");
    return QLatin1String("");
}

inline QString SyslogAnalyzer::undefinedProcess()
{
    // i18nc("Undefined process", "undefined");
    return QLatin1String("");
}
