/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Cups Log Mode Identifier
 */
#define CUPS_LOG_MODE_ID "cupsLogMode"

/**
 * Cups Log Icon
 */
#define CUPS_MODE_ICON "preferences-devices-printer"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class CupsConfiguration;
class CupsConfigurationWidget;

class CupsLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit CupsLogMode(QSharedPointer<CupsConfiguration> &cupsConfiguration, CupsConfigurationWidget *cupsConfigurationWidget);

    ~CupsLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

