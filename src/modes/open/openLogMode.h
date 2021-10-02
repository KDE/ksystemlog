/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

/**
 * Open Log Mode Identifier
 */
#define OPEN_LOG_MODE_ID "openLogMode"

/**
 * System Log Icon
 */
#define OPEN_MODE_ICON "document-open"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class QWidget;

class OpenLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit OpenLogMode(QWidget *parent);

    ~OpenLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;

private:
    QWidget *const mParent;
};

