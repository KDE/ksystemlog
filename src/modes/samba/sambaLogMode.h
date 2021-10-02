/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Samba Log Mode Identifier
 */
#define SAMBA_LOG_MODE_ID "sambaLogMode"

/**
 * Samba Log Icon
 */
#define SAMBA_MODE_ICON "preferences-system-network-share-windows"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class SambaConfiguration;
class SambaConfigurationWidget;
class SambaItemBuilder;

class SambaLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit SambaLogMode(QSharedPointer<SambaConfiguration> &sambaConfiguration,
                          SambaConfigurationWidget *sambaConfigurationWidget,
                          SambaItemBuilder *itemBuilder);

    ~SambaLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

