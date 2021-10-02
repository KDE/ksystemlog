/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

/**
 * Apache Log Mode Identifier
 */
#define APACHE_LOG_MODE_ID "apacheLogMode"

/**
 * Apache Log Icon
 */
#define APACHE_MODE_ICON "preferences-system-network"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class ApacheConfiguration;
class ApacheConfigurationWidget;

class ApacheLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit ApacheLogMode(QSharedPointer<ApacheConfiguration> &apacheConfiguration, ApacheConfigurationWidget *apacheConfigurationWidget);

    ~ApacheLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

