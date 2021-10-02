/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "ksystemlog_debug.h"

#include "localLogFileReader.h"
#include "parsingHelper.h"
#include "sambaLogMode.h"

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

