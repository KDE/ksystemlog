/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * System Log Mode Identifier
 */
#define SYSTEM_LOG_MODE_ID "systemLogMode"

/**
 * System Log Icon
 */
#define SYSTEM_MODE_ICON "preferences-desktop-wallpaper"

#include <QList>

#include "logFile.h"
#include "logMode.h"

class SystemLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit SystemLogMode();

    ~SystemLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

