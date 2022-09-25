/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Daemon Log Mode Identifier
 */
#define DAEMON_LOG_MODE_ID "daemonLogMode"

/**
 * Daemon Log Icon
 */
#define DAEMON_MODE_ICON "utilities-terminal"

#include "ksystemlog_debug.h"
#include "logMode.h"

#include "daemonConfiguration.h"
#include "daemonConfigurationWidget.h"
#include "syslogAnalyzer.h"

#include "logModeItemBuilder.h"

class DaemonLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit DaemonLogMode();

    ~DaemonLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};
