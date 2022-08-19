/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Apache Access Log Mode Identifier
 */
#define APACHE_ACCESS_LOG_MODE_ID "apacheAccessLogMode"

/**
 * Apache Access Log Icon
 */
#define APACHE_ACCESS_MODE_ICON "preferences-system-network-server-web"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class ApacheConfiguration;
class ApacheConfigurationWidget;

class ApacheAccessLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit ApacheAccessLogMode(QSharedPointer<ApacheConfiguration> &apacheConfiguration, ApacheConfigurationWidget *apacheConfigurationWidget);

    ~ApacheAccessLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};
