/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "parsingHelper.h"
#include "xorgLogMode.h"

#define CONFIG_FILE_LOG_LEVEL_ICON "configure"
#define DEFAULT_SETTING_LOG_LEVEL_ICON "configure-toolbars"
#define COMMAND_LINE_LOG_LEVEL_ICON "konsole"
#define PROBED_LOG_LEVEL_ICON "favorites"
#define NOT_IMPLEMENTED_LOG_LEVEL_ICON "document-new"

class XorgAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit XorgAnalyzer(LogMode *logMode);

    ~XorgAnalyzer() override;

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile) override;

private:
    void initializeTypeName();

    LogLevel *findTypeName(const QString &type);

    QMap<QString, LogLevel *> mXorgLevels;
    const QDateTime mCurrentDateTime;
};

