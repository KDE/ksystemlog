/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QList>
#include <QObject>

class LogModeAction;
class LogMode;

class LogModeFactory : public QObject
{
    Q_OBJECT

public:
    explicit LogModeFactory();

    ~LogModeFactory() override;

    virtual LogModeAction *createLogModeAction() const = 0;

    /**
     * Create the log mode
     */
    virtual QList<LogMode *> createLogModes() const = 0;
};

