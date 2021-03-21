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

#pragma once

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "logging.h"
#include "parsingHelper.h"

#include "cupsPageLogMode.h"

class CupsPageAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit CupsPageAnalyzer(LogMode *logMode);

    ~CupsPageAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    const QRegExp mCupsPageRegex;

    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    /*
     * https://www.cups.org/doc/man-cupsd-logs.html
     * Format : printer user job-id date-time page-number num-copies job-billing
     *
     * DeskJet root 2 [20/May/1999:19:21:05 +0000] 1 1 acme-123
     * DeskJet root 2 [20/May/1999:19:21:05 +0000] 2 1 acme-123
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;
};

