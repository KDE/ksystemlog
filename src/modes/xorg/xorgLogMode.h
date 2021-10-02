/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Xorg Log Mode Identifier
 */
#define XORG_LOG_MODE_ID "xorgLogMode"

/**
 * Xorg Log Icon
 */
#define XORG_MODE_ICON "xorg"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class XorgLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit XorgLogMode();

    ~XorgLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

