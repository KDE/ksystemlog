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

#ifndef CUPS_ACCESS_ANALYZER_H
#define CUPS_ACCESS_ANALYZER_H

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "logging.h"
#include "parsingHelper.h"

#include "cupsAccessLogMode.h"

class CupsAccessAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit CupsAccessAnalyzer(LogMode *logMode);

    ~CupsAccessAnalyzer() override {}

    LogViewColumns initColumns() override;

protected:
    QRegExp mCupsAccessRegex;

    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    /*
     * https://www.cups.org/doc/man-cupsd-logs.html
     *
     * host group user date-time \"method resource version\" status bytes ipp-operation ipp-status
     * 10.0.1.2 - - [01/Dec/2005:21:50:28 +0000] "POST / HTTP/1.1" 200 317 CUPS-Get-Printers
     *successful-ok-ignored-or-substituted-attributes
     * localhost - - [01/Dec/2005:21:50:32 +0000] "GET /admin HTTP/1.1" 200 0 - -
     * localhost - - [01/Dec/2005:21:50:32 +0000] "POST / HTTP/1.1" 200 157 CUPS-Get-Printers
     *successful-ok-ignored-or-substituted-attributes
     * localhost - - [01/Dec/2005:21:50:32 +0000] "POST / HTTP/1.1" 200 1411 CUPS-Get-Devices -
     * localhost - - [01/Dec/2005:21:50:32 +0000] "GET /admin HTTP/1.1" 200 6667 - -
     *
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

    inline LogLevel *findLevel(const QString &status) const
    {
        if (status == QLatin1String("successful-ok"))
            return Globals::instance().informationLogLevel();
        else if (status == QLatin1String("ignored"))
            return Globals::instance().warningLogLevel();

        return Globals::instance().noticeLogLevel();
    }
};

#endif // _CUPS_ACCESS_ANALYZER_H
