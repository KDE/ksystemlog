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

#ifndef APACHE_ACCESS_ANALYZER_H
#define APACHE_ACCESS_ANALYZER_H

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "logging.h"
#include "parsingHelper.h"

#include "apacheAccessLogMode.h"

class ApacheAccessAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit ApacheAccessAnalyzer(LogMode *logMode)
        : FileAnalyzer(logMode)
    {
    }

    ~ApacheAccessAnalyzer() override {}

    LogViewColumns initColumns() override
    {
        LogViewColumns columns;

        columns.addColumn(LogViewColumn(i18n("Date"), true, false));
        columns.addColumn(LogViewColumn(i18n("Host Name"), true, true));
        columns.addColumn(LogViewColumn(
            i18n("Id."), true, true)); //=Identification protocol [From RFC1413 (see Google for more infos)]
        columns.addColumn(LogViewColumn(i18n("User"), true, true));
        columns.addColumn(LogViewColumn(i18n("Response"), true, true));
        columns.addColumn(LogViewColumn(i18n("Bytes Sent"), true, false));
        columns.addColumn(LogViewColumn(i18n("Agent Identity"), true, true));
        columns.addColumn(LogViewColumn(i18n("HTTP Request"), true, false));
        columns.addColumn(LogViewColumn(i18n("URL"), true, true));

        return columns;
    }

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override { return new LocalLogFileReader(logFile); }

    Analyzer::LogFileSortMode logFileSortMode() override { return Analyzer::AscendingSortedLogFile; }

    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override
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

        QDateTime dateTime
            = ParsingHelper::instance()->parseHttpDateTime(strDateTime.mid(1, strDateTime.count() - 2));

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

        return new LogLine(logLineInternalIdGenerator++, dateTime, list, originalLogFile.url().toLocalFile(),
                           Globals::instance().informationLogLevel(), logMode);
    }
};

#endif // _APACHE_ACCESS_ANALYZER_H
