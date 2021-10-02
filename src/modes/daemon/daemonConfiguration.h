/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "logging.h"

#include "ksystemlogConfig.h"

class DaemonConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    DaemonConfiguration();

    ~DaemonConfiguration() override;

    QStringList daemonPaths() const;

    void setDaemonPaths(const QStringList &daemonPaths);

private:
    QStringList mDaemonPaths;
};

