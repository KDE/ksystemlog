/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LogModePluginsLoader : public QObject
{
    Q_OBJECT

public:
    explicit LogModePluginsLoader(QWidget *parent);

    ~LogModePluginsLoader() override;

    void loadPlugins();

private:
    QWidget *const mParent;
};

