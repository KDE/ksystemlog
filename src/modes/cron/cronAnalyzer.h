/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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

