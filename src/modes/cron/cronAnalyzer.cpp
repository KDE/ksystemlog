/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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

    if (!isCronLine(syslogLine)) {
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
