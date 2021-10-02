/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "logging.h"

#include "apacheLogMode.h"
#include "localLogFileReader.h"
#include "parsingHelper.h"

class ApacheAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit ApacheAnalyzer(LogMode *logMode);

    ~ApacheAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    /*
     * Log line examples :
     * [Wed May 18 22:16:02 2005] [error] [client 127.0.0.1] File does not exist:
     * /var/www/html/ksystemlog/screenshots/small/kernel-view.png, referer:
     * http://localhost.localdomain/ksystemlog/screenshots.php
     * [Wed May 18 22:16:02 2005] [error] [client 127.0.0.1] File does not exist:
     * /var/www/html/ksystemlog/screenshots/small/system-filter.png, referer:
     * http://localhost.localdomain/ksystemlog/screenshots.php
     * [Thu May 19 18:00:19 2005] [notice] mod_jk2.post_config() first invocation
     * [Thu May 19 18:00:19 2005] [notice] Digest: generating secret for digest authentication ...
     * [client 127.0.0.1] PHP Parse error:  parse error, unexpected T_PRIVATE, expecting T_STRING in
     * /mnt/boulot/web/annivernet/src/fonctions/formulaire.inc.php on line 25
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

private:
    QMap<QString, LogLevel *> mMapTypeLevels;

    void initializeTypeLevels();

    LogLevel *findLogLevel(const QString &type);
};

