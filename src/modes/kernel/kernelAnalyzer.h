/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QFile>
#include <QRegExp>

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "fileAnalyzer.h"

#include "kernelLogMode.h"
#include "localLogFileReader.h"
#include "processOutputLogFileReader.h"

class LogMode;

class KernelAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit KernelAnalyzer(LogMode *logMode);

    ~KernelAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override
    {
        return Analyzer::AscendingSortedLogFile;
    }

    void startupTime();

    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

    inline void parseComponentMessage(const QString &logLine, QStringList &messages)
    {
        QString message(logLine);
        QString component;

        int doublePointPosition = message.indexOf(QLatin1Char(':'));

        // Estimate the max size of a component
        if (doublePointPosition != -1 && doublePointPosition < 20) {
            component = message.left(doublePointPosition);
            // Remove component length + ": "
            message.remove(0, doublePointPosition + 2);
        }

        messages.append(component);
        messages.append(message.simplified());
    }

protected:
    QDateTime mStartupDateTime;
};

