/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QList>

#include "logModeFactory.h"

class SambaLogModeFactory : public LogModeFactory
{
    Q_OBJECT

public:
    QList<LogMode *> createLogModes() const override;

    LogModeAction *createLogModeAction() const override;
};

