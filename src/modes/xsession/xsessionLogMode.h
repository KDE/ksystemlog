/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Xorg Log Mode Identifier
 */
#define X_SESSION_LOG_MODE_ID "xsessionLogMode"

/**
 * Xorg Log Icon
 */
#define X_SESSION_MODE_ICON "preferences-system-windows-effect-coverswitch"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class XSessionLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit XSessionLogMode();

    ~XSessionLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

