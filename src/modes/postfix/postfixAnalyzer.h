/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "syslogAnalyzer.h"

#include "logMode.h"
#include "logging.h"

class PostfixAnalyzer : public SyslogAnalyzer
{
    Q_OBJECT

public:
    explicit PostfixAnalyzer(LogMode *logMode);

    ~PostfixAnalyzer() override
    {
    }

    /*
     * Just a test of multilines log lines (and it works well !)
     */
    /*
    LogLine* parseMessage(const QString& logLine, const LogFile& originalFile) {
        LogLine* syslogLine = SyslogAnalyzer::parseMessage(logLine, originalFile);

        QStringList items = syslogLine->logItems();
        QString message = items.takeLast();

        items.append(message + "\n" + message);

  logDebug() << "Coucou" << items.at(items.count()-1);

        syslogLine->setLogItems(items);
        return syslogLine;
    }
    */
};

