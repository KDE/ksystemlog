/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QList>
#include <QObject>
#include <QStringList>

#include "logFile.h"

class KSystemLogConfig;

class LogModeConfiguration : public QObject
{
    Q_OBJECT

public:
    explicit LogModeConfiguration(QObject *parent = nullptr);

    ~LogModeConfiguration() override;

    LogFile findGenericLogFile(const QString &file);
    QVector<LogFile> findGenericLogFiles(const QStringList &files);

    QVector<LogFile> findNoModeLogFiles(const QStringList &stringList);

protected:
    KSystemLogConfig *mConfiguration = nullptr;
};

