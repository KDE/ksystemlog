/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "parsingHelper.h"

#include "xsessionConfiguration.h"
#include "xsessionLogMode.h"

class XSessionAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit XSessionAnalyzer(LogMode *logMode);

    ~XSessionAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override
    {
        return new LocalLogFileReader(logFile);
    }

    Analyzer::LogFileSortMode logFileSortMode() override;

    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile) override;

private:
    bool isXorgError(const QString &program);

    bool hasWarningKeywords(const QString &message);

    bool hasErrorKeywords(const QString &message);

    bool hasKeywords(const QString &message, const QStringList &keywords);

    QDateTime mCurrentDateTime;
};

