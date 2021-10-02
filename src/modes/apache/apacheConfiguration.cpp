/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "apacheConfiguration.h"

ApacheConfiguration::ApacheConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("ApacheLogMode"));

    const QStringList defaultApachePaths{QStringLiteral("/var/log/apache2/error.log")};
    mConfiguration->addItemStringList(QStringLiteral("ApacheLogFilesPaths"), mApachePaths, defaultApachePaths, QStringLiteral("ApacheLogFilesPaths"));

    const QStringList defaultApacheAccessPaths{QStringLiteral("/var/log/apache2/access.log")};
    mConfiguration->addItemStringList(QStringLiteral("ApacheAccessLogFilesPaths"),
                                      mApacheAccessPaths,
                                      defaultApacheAccessPaths,
                                      QStringLiteral("ApacheAccessLogFilesPaths"));
}

ApacheConfiguration::~ApacheConfiguration()
{
}

QStringList ApacheConfiguration::apachePaths() const
{
    return mApachePaths;
}

QStringList ApacheConfiguration::apacheAccessPaths() const
{
    return mApacheAccessPaths;
}

void ApacheConfiguration::setApachePaths(const QStringList &apachePaths)
{
    mApachePaths = apachePaths;
}

void ApacheConfiguration::setApacheAccessPaths(const QStringList &apacheAccessPaths)
{
    mApacheAccessPaths = apacheAccessPaths;
}
