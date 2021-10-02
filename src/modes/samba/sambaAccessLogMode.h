/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

/**
 * Samba Access Log Mode Identifier
 */
#define SAMBA_ACCESS_LOG_MODE_ID "sambaAccessLogMode"

/**
 * Samba Access Log Icon
 */
#define SAMBA_ACCESS_MODE_ICON "preferences-system-network-server-share-windows"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class SambaConfiguration;
class SambaConfigurationWidget;
class SambaItemBuilder;

class SambaAccessLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit SambaAccessLogMode(QSharedPointer<SambaConfiguration> &sambaConfiguration,
                                SambaConfigurationWidget *sambaConfigurationWidget,
                                SambaItemBuilder *itemBuilder);

    ~SambaAccessLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

