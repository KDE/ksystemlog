/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "logging.h"

#include "apacheLogMode.h"

#include "ksystemlogConfig.h"

class ApacheConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    ApacheConfiguration();

    ~ApacheConfiguration() override;

    QStringList apachePaths() const;

    QStringList apacheAccessPaths() const;

    void setApachePaths(const QStringList &apachePaths);

    void setApacheAccessPaths(const QStringList &apacheAccessPaths);

private:
    QStringList mApachePaths;

    QStringList mApacheAccessPaths;
};

