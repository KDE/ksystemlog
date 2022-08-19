/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "ksystemlog_debug.h"

#include "cronLogMode.h"

#include "ksystemlogConfig.h"

class CronConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    CronConfiguration();

    ~CronConfiguration() override;

    QString processFilter() const;

    void setProcessFilter(const QString &processFilter);

    QStringList cronPaths() const;

    void setCronPaths(const QStringList &cronPaths);

private:
    QStringList mCronPaths;

    QString mProcessFilter;
};
