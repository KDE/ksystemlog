/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Cron Log Mode Identifier
 */
#define CRON_LOG_MODE_ID "cronLogMode"

/**
 * Cron Log Icon
 */
#define CRON_MODE_ICON "preferences-system-time"

#include <QList>

#include "logFile.h"
#include "logMode.h"

class CronLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit CronLogMode();

    ~CronLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

