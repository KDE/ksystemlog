/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "logModeConfiguration.h"
#include "logModeFactory.h"

class OpenLogModeFactory : public LogModeFactory
{
    Q_OBJECT

public:
    explicit OpenLogModeFactory(QWidget *parent);

    ~OpenLogModeFactory() override;

    QList<LogMode *> createLogModes() const override;

    LogModeAction *createLogModeAction() const override;

private:
    QWidget *const mParent;
};

