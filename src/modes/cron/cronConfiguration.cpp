/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cronConfiguration.h"

CronConfiguration::CronConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("CronLogMode"));

    const QStringList defaultCronPaths{QStringLiteral("/var/log/syslog")};
    mConfiguration->addItemStringList(QStringLiteral("LogFilesPaths"), mCronPaths, defaultCronPaths, QStringLiteral("LogFilesPaths"));

    const QString defaultProcessFilter(QStringLiteral("/usr/sbin/cron"));
    mConfiguration->addItemString(QStringLiteral("ProcessFilter"), mProcessFilter, defaultProcessFilter, QStringLiteral("ProcessFilter"));
}

CronConfiguration::~CronConfiguration()
{
}

QString CronConfiguration::processFilter() const
{
    return mProcessFilter;
}

void CronConfiguration::setProcessFilter(const QString &processFilter)
{
    mProcessFilter = processFilter;
}

QStringList CronConfiguration::cronPaths() const
{
    return mCronPaths;
}

void CronConfiguration::setCronPaths(const QStringList &cronPaths)
{
    mCronPaths = cronPaths;
}
