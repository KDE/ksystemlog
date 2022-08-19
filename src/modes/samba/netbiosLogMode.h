/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later

*/

#pragma once

/**
 * Netbios Log Mode Identifier
 */
#define NETBIOS_LOG_MODE_ID "netbiosLogMode"

/**
 * Netbios Log Icon
 */
#define NETBIOS_MODE_ICON "folder-remote"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class SambaConfiguration;
class SambaConfigurationWidget;
class SambaItemBuilder;

class NetbiosLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit NetbiosLogMode(QSharedPointer<SambaConfiguration> &sambaConfiguration,
                            SambaConfigurationWidget *sambaConfigurationWidget,
                            SambaItemBuilder *itemBuilder);

    ~NetbiosLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};
