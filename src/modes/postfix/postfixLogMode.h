/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Postfix Log Mode Identifier
 */
#define POSTFIX_LOG_MODE_ID "postfixLogMode"

/**
 * Postfix Log Icon
 */
#define POSTFIX_MODE_ICON "internet-mail"

#include <QList>

#include "logFile.h"
#include "logMode.h"

class PostfixLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit PostfixLogMode();

    ~PostfixLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

