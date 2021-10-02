/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "apacheAccessAnalyzer.h"

ApacheAccessAnalyzer::ApacheAccessAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
}

LogViewColumns ApacheAccessAnalyzer::initColumns()
{
    LogViewColumns columns;

    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Host Name"), true, true));
    columns.addColumn(LogViewColumn(i18n("Id."), true, true)); //=Identification protocol [From RFC1413 (see Google for more infos)]
    columns.addColumn(LogViewColumn(i18n("User"), true, true));
    columns.addColumn(LogViewColumn(i18n("Response"), true, true));
    columns.addColumn(LogViewColumn(i18n("Bytes Sent"), true, false));
    columns.addColumn(LogViewColumn(i18n("Agent Identity"), true, true));
    columns.addColumn(LogViewColumn(i18n("HTTP Request"), true, false));
    columns.addColumn(LogViewColumn(i18n("URL"), true, true));

    return columns;
}

LogFileReader *ApacheAccessAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode ApacheAccessAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

LogLine *ApacheAccessAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    QString line(logLine);

    int spacePos = line.indexOf(QLatin1Char(' '));

    QString hostName = line.left(spacePos);
    line.remove(0, spacePos + 1);

    spacePos = line.indexOf(QLatin1Char(' '));
    QString identd = line.left(spacePos);
    line.remove(0, spacePos + 1);

    spacePos = line.indexOf(QLatin1Char(' '));
    QString userName = line.left(spacePos);
    line.remove(0, spacePos + 1);

    int endDate = line.indexOf(QLatin1Char(']'));
    QString strDateTime = line.left(endDate);
    line.remove(0, endDate + 3);

    QDateTime dateTime = ParsingHelper::instance()->parseHttpDateTime(strDateTime.mid(1, strDateTime.count() - 2));

    int endQuote = line.indexOf(QLatin1Char('\"'));
    QString message = line.left(endQuote);
    line.remove(0, endQuote + 2);

    spacePos = line.indexOf(QLatin1Char(' '));
    QString httpResponse = ParsingHelper::instance()->parseHttpResponse(line.left(spacePos));
    line.remove(0, spacePos + 1);

    spacePos = line.indexOf(QLatin1Char(' '));
    QString bytesSent = ParsingHelper::instance()->parseSize(line.left(spacePos));
    line.remove(0, spacePos + 2);

    QString url;

    endQuote = line.indexOf(QLatin1Char('\"'));
    if (endQuote != -1) {
        url = line.left(endQuote);
        line.remove(0, endQuote + 3);
    }

    QString agent;

    // TODO Convert this value to find a more simple name for the Agent
    endQuote = line.indexOf(QLatin1Char('\"'));
    if (endQuote != -1) {
        agent = ParsingHelper::instance()->parseAgent(line.left(endQuote));
    }

    QStringList list;
    list.append(hostName);
    list.append(identd);
    list.append(userName);
    list.append(httpResponse);
    list.append(bytesSent);
    list.append(agent);
    list.append(message);
    list.append(url);

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, list, originalLogFile.url().toLocalFile(), Globals::instance().informationLogLevel(), mLogMode);
}
