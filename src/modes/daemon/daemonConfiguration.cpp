/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "daemonConfiguration.h"

DaemonConfiguration::DaemonConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("DaemonLogMode"));

    const QStringList defaultDaemonPaths{QStringLiteral("/var/log/daemon.log")};
    mConfiguration->addItemStringList(QStringLiteral("LogFilesPaths"), mDaemonPaths, defaultDaemonPaths, QStringLiteral("LogFilesPaths"));
}

DaemonConfiguration::~DaemonConfiguration()
{
}

QStringList DaemonConfiguration::daemonPaths() const
{
    return mDaemonPaths;
}

void DaemonConfiguration::setDaemonPaths(const QStringList &daemonPaths)
{
    mDaemonPaths = daemonPaths;
}
