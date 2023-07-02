/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "authenticationAnalyzer.h"

AuthenticationAnalyzer::AuthenticationAnalyzer(LogMode *logMode)
    : SyslogAnalyzer(logMode)
{
}

LogLine *AuthenticationAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    LogLine *syslogLine = SyslogAnalyzer::parseMessage(logLine, originalLogFile);

    const QString message = syslogLine->logItems().at(syslogLine->logItems().count() - 1);

    if (hasErrorKeywords(message)) {
        syslogLine->setLogLevel(Globals::instance().errorLogLevel());
    } else if (hasWarningKeywords(message)) {
        syslogLine->setLogLevel(Globals::instance().warningLogLevel());
    }

    return syslogLine;
}

bool AuthenticationAnalyzer::hasWarningKeywords(const QString &message)
{
    auto *configuration = mLogMode->logModeConfiguration<AuthenticationConfiguration *>();
    return hasKeywords(message, configuration->warningKeywords());
}

bool AuthenticationAnalyzer::hasErrorKeywords(const QString &message)
{
    auto *configuration = mLogMode->logModeConfiguration<AuthenticationConfiguration *>();
    return hasKeywords(message, configuration->errorKeywords());
}

bool AuthenticationAnalyzer::hasKeywords(const QString &message, const QStringList &keywords)
{
    for (const QString &keyword : keywords) {
        if (message.contains(keyword, Qt::CaseInsensitive)) {
            return true;
        }
    }

    return false;
}

#include "moc_authenticationAnalyzer.cpp"
