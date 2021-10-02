/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include <QList>
#include <QStringList>

#include "logFile.h"

#include "logModeConfiguration.h"

class GenericLogModeConfigurationPrivate;

class GenericLogModeConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    GenericLogModeConfiguration(const QString &configurationGroup, const QStringList &defaultLogFilesPaths, const QList<int> &defaultLogFilesLevels);
    ~GenericLogModeConfiguration() override;

    QStringList logFilesPaths() const;

    QList<int> logFilesLevels() const;

    void setLogFilesPaths(const QStringList &logFilesPaths);

    void setLogFilesLevels(const QList<int> &logFilesLevels);

    QVector<LogFile> findGenericLogFiles() const;

private:
    QStringList mLogFilesPaths;

    QList<int> mLogFilesLevels;
};

