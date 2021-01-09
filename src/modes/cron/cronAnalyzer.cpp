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

#include "cronAnalyzer.h"

LogViewColumns CronAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Host"), true, true));
    columns.addColumn(LogViewColumn(i18n("Process"), true, true));
    columns.addColumn(LogViewColumn(i18n("User"), true, true));
    columns.addColumn(LogViewColumn(i18n("Command"), true, false));
    return columns;
}

Analyzer::LogFileSortMode CronAnalyzer::logFileSortMode()
{
    return Analyzer::FilteredLogFile;
}

LogLine *CronAnalyzer::parseMessage(const QString &logLine, const LogFile &originalFile)
{
    // Use the default parsing
    LogLine *syslogLine = SyslogAnalyzer::parseMessage(logLine, originalFile);

    QStringList list = syslogLine->logItems();

    if (isCronLine(syslogLine) == false) {
        delete syslogLine;
        return nullptr;
    }

    // Gets the message column (last item) and deletes it
    QString message = list.takeLast();

    int leftBracket = message.indexOf(QLatin1Char('('));
    int rightBracket = message.indexOf(QLatin1Char(')'));

    const QString user = message.mid(leftBracket + 1, rightBracket - leftBracket - 1);

    list.append(user);

    if (message.indexOf(QLatin1String("CMD")) != -1) {
        // Ignore this : ") CMD (" (length = 7)
        message = message.right(message.length() - rightBracket - 7);
        message = message.simplified();
        syslogLine->setLogLevel(Globals::instance().informationLogLevel());
    } else {
        // Ignore this : ") " (for INFO and STARTUP cases)
        message = message.right(message.length() - rightBracket - 2);
        syslogLine->setLogLevel(Globals::instance().noticeLogLevel());
    }

    list.append(message);

    syslogLine->setLogItems(list);

    return syslogLine;
}

CronAnalyzer::CronAnalyzer(LogMode *logMode)
    : SyslogAnalyzer(logMode)
{
}
