/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Acpid Log Mode Identifier
 */
#define AUTHENTICATION_LOG_MODE_ID "authenticationLogMode"

/**
 * Authentication Log Icon
 */
#define AUTHENTICATION_MODE_ICON "dialog-password"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class AuthenticationLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit AuthenticationLogMode();

    ~AuthenticationLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

