/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Acpid Log Mode Identifier
 */
#define ACPID_LOG_MODE_ID "acpidLogMode"

/**
 * Acpid Log Icon
 */
#define ACPID_MODE_ICON "preferences-system-power-management"

#include "logFile.h"

#include "logMode.h"

class AcpidLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit AcpidLogMode();

    ~AcpidLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};
