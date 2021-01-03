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

#include "cronLogMode.h"
#include "cronConfiguration.h"

class LogMode;

class CronAnalyzer : public SyslogAnalyzer
{
    Q_OBJECT

public:
    explicit CronAnalyzer(LogMode *logMode)
        : SyslogAnalyzer(logMode)
    {
    }

    ~CronAnalyzer() override {}

    LogViewColumns initColumns() override
    {
        LogViewColumns columns;
        columns.addColumn(LogViewColumn(i18n("Date"), true, false));
        columns.addColumn(LogViewColumn(i18n("Host"), true, true));
        columns.addColumn(LogViewColumn(i18n("Process"), true, true));
        columns.addColumn(LogViewColumn(i18n("User"), true, true));
        columns.addColumn(LogViewColumn(i18n("Command"), true, false));
        return columns;
    }

    Analyzer::LogFileSortMode logFileSortMode() override { return Analyzer::FilteredLogFile; }

    /*
     * Cron line example :
     * Sep 16 01:39:01 localhost /USR/SBIN/CRON[11069]: (root) CMD (  [ -d /var/lib/php5 ] && find /var/lib/php5/ -type f -cmin +$(/usr/lib/php5/maxlifetime) -print0 | xargs -r -0 rm)
     * Sep 16 18:39:05 localhost /usr/sbin/cron[5479]: (CRON) INFO (pidfile fd = 3)
     * Sep 16 18:39:05 localhost /usr/sbin/cron[5480]: (CRON) STARTUP (fork ok)
     * Sep 16 18:39:05 localhost /usr/sbin/cron[5480]: (CRON) INFO (Running @reboot jobs)
     *
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile) override
    {
        // Use the default parsing
        LogLine *syslogLine = SyslogAnalyzer::parseMessage(logLine, originalFile);

        QStringList list = syslogLine->logItems();

        if (isCronLine(syslogLine) == false) {
            delete syslogLine;
            return nullptr;
        }

        // Gets the message column (last item) and deletes it
        QString message = list.takeLast();

        int leftBracket = message.indexOf(QLatin1Char('('));
        int rightBracket = message.indexOf(QLatin1Char(')'));

        QString user = message.mid(leftBracket + 1, rightBracket - leftBracket - 1);

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

    inline bool isCronLine(LogLine *syslogLine)
    {
        CronConfiguration *cronConfiguration = logMode->logModeConfiguration<CronConfiguration *>();
        if (cronConfiguration->processFilter().isEmpty()) {
            return true;
        }

        // If the process line does not match the cron process, then ignore this line
        const QStringList list = syslogLine->logItems();
        QString processLine = list.at(1);
        if (processLine.contains(cronConfiguration->processFilter(), Qt::CaseInsensitive) == true) {
            return true;
        }

        return false;
    }
};

#endif
