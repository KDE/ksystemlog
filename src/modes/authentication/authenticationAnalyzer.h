/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include "syslogAnalyzer.h"

#include "authenticationConfiguration.h"
#include "authenticationLogMode.h"

class AuthenticationAnalyzer : public SyslogAnalyzer
{
    Q_OBJECT

public:
    explicit AuthenticationAnalyzer(LogMode *logMode);

    ~AuthenticationAnalyzer() override
    {
    }

    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

private:
    bool hasWarningKeywords(const QString &message);

    bool hasErrorKeywords(const QString &message);

    bool hasKeywords(const QString &message, const QStringList &keywords);
};

