/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sambaConfiguration.h"

SambaConfiguration::SambaConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("SambaLogMode"));

    const QStringList defaultSambaPaths{QStringLiteral("/var/log/samba/log.smbd")};
    mConfiguration->addItemStringList(QStringLiteral("SambaLogFilesPaths"), mSambaPaths, defaultSambaPaths, QStringLiteral("SambaLogFilesPaths"));

    QStringList defaultSambaAccessPaths;
    defaultSambaAccessPaths << QStringLiteral("/var/log/samba/log.localhost") << QStringLiteral("/var/log/samba/log.127.0.0.1");
    mConfiguration->addItemStringList(QStringLiteral("SambaAccessLogFilesPaths"),
                                      mSambaAccessPaths,
                                      defaultSambaAccessPaths,
                                      QStringLiteral("SambaAccessLogFilesPaths"));

    const QStringList defaultNetbiosPaths{QStringLiteral("/var/log/samba/log.nmbd")};
    mConfiguration->addItemStringList(QStringLiteral("NetbiosLogFilesPaths"), mNetbiosPaths, defaultNetbiosPaths, QStringLiteral("NetbiosLogFilesPaths"));
}

SambaConfiguration::~SambaConfiguration()
{
}

QStringList SambaConfiguration::sambaPaths() const
{
    return mSambaPaths;
}

QStringList SambaConfiguration::sambaAccessPaths() const
{
    return mSambaAccessPaths;
}

QStringList SambaConfiguration::netbiosPaths() const
{
    return mNetbiosPaths;
}

void SambaConfiguration::setSambaPaths(const QStringList &sambaPaths)
{
    mSambaPaths = sambaPaths;
}

void SambaConfiguration::setNetbiosPaths(const QStringList &netbiosPaths)
{
    mNetbiosPaths = netbiosPaths;
}

void SambaConfiguration::setSambaAccessPaths(const QStringList &sambaAccessPaths)
{
    mSambaAccessPaths = sambaAccessPaths;
}
