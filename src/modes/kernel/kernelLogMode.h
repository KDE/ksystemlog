/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Kernel Log Mode Identifier
 */
#define KERNEL_LOG_MODE_ID "kernelLogMode"

/**
 * Kernel Log Icon
 */
#define KERNEL_MODE_ICON "preferences-system-linux"

#define UPTIME_FILE "/proc/uptime"

#include <QList>

#include "logFile.h"
#include "logMode.h"

class KernelLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit KernelLogMode();

    ~KernelLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

