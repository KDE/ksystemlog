/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Cups Access Log Mode Identifier
 */
#define CUPS_ACCESS_LOG_MODE_ID "cupsAccessLogMode"

/**
 * Cups Access Log Icon
 */
#define CUPS_ACCESS_MODE_ICON "preferences-devices-printer"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class CupsConfiguration;
class CupsConfigurationWidget;

class CupsAccessLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit CupsAccessLogMode(QSharedPointer<CupsConfiguration> &cupsConfiguration, CupsConfigurationWidget *cupsConfigurationWidget);

    ~CupsAccessLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

