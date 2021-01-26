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

#ifndef CRON_ANALYZER_H
#define CRON_ANALYZER_H

#include <KLocalizedString>

#include "syslogAnalyzer.h"

#include "cronConfiguration.h"
#include "cronLogMode.h"

class LogMode;

class CronAnalyzer : public SyslogAnalyzer
{
    Q_OBJECT

public:
    explicit CronAnalyzer(LogMode *logMode);
    ~CronAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    /*
     * Cron line example :
     * Sep 16 01:3;D (  [ -d /var/lib/php5 ] && find /var/lib/php5/ -type f -cmin +$(/usr/lib/php5/maxlifetime) -print0 | xargs -r -0 rm)
     * Sep 16 18:39:05 localhost /usr/sbin/cron[5479]: (CRON) INFO (pidfile fd = 3)
     * Sep 16 18:39:05 localhost /usr/sbin/cron[5480]: (CRON) STARTUP (fork ok)
     * Sep 16 18:39:05 localhost /usr/sbin/cron[5480]: (CRON) INFO (Running @reboot jobs)
     *
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile) override;

    inline bool isCronLine(LogLine *syslogLine)
    {
        auto *cronConfiguration = mLogMode->logModeConfiguration<CronConfiguration *>();
        if (cronConfiguration->processFilter().isEmpty()) {
            return true;
        }

        // If the process line does not match the cron process, then ignore this line
        const QStringList list = syslogLine->logItems();
        QString processLine = list.at(1);
        if (processLine.contains(cronConfiguration->processFilter(), Qt::CaseInsensitive)) {
            return true;
        }

        return false;
    }
};

#endif
