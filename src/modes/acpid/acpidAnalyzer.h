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

#ifndef ACPID_ANALYZER_H
#define ACPID_ANALYZER_H

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "parsingHelper.h"
#include "acpidLogMode.h"

class AcpidAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit AcpidAnalyzer(LogMode *logMode);

    ~AcpidAnalyzer() override {}

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override { return new LocalLogFileReader(logFile); }

    Analyzer::LogFileSortMode logFileSortMode() override { return Analyzer::AscendingSortedLogFile; }

    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile) override;
};

#endif // _ACPID_ANALYZER_H
