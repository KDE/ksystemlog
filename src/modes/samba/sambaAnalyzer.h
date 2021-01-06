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

#ifndef SAMBA_ANALYZER_H
#define SAMBA_ANALYZER_H

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "logging.h"

#include "localLogFileReader.h"
#include "sambaLogMode.h"
#include "parsingHelper.h"

class SambaAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit SambaAnalyzer(LogMode *logMode);

    ~SambaAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    /*
     * Log line examples :
     * [2005/06/27 21:06:01, 0] nmbd/nmbd.c:main(668)
     * Netbios nameserver version 3.0.14a started.
     * Copyright Andrew Tridgell and the Samba Team 1994-2004
     * [2005/06/27 21:11:46, 0] nmbd/nmbd_become_lmb.c:become_local_master_stage2(396)
     *  *****
     *  Samba name server STEAKHACHE is now a local master browser for workgroup MAISON on subnet 192.168.1.33
     *
     *  *****
     * [2005/06/28 06:41:03, 0] nmbd/nmbd.c:terminate(56)
     * Got SIGTERM: going down...
     * [2005/06/28 18:08:11, 0] nmbd/nmbd.c:main(668)
     * Netbios nameserver version 3.0.14a started.
     * Copyright Andrew Tridgell and the Samba Team 1994-2004
     *
     * Note:
     * This analyzer nevers return the last line of a log file because it's never sure
     * that the last file line is the last message of the current log line.
     * So the previous last line will be returned at the next file update,
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

    LogLine *mCurrentLogLine;
};

#endif // _SAMBA_ANALYZER_H
