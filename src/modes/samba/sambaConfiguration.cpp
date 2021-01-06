/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "sambaConfiguration.h"

SambaConfiguration::SambaConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("SambaLogMode"));

    QStringList defaultSambaPaths;
    defaultSambaPaths << QStringLiteral("/var/log/samba/log.smbd");
    mConfiguration->addItemStringList(QStringLiteral("SambaLogFilesPaths"), mSambaPaths,
                                      defaultSambaPaths, QStringLiteral("SambaLogFilesPaths"));

    QStringList defaultSambaAccessPaths;
    defaultSambaAccessPaths << QStringLiteral("/var/log/samba/log.localhost")
                            << QStringLiteral("/var/log/samba/log.127.0.0.1");
    mConfiguration->addItemStringList(QStringLiteral("SambaAccessLogFilesPaths"), mSambaAccessPaths,
                                      defaultSambaAccessPaths, QStringLiteral("SambaAccessLogFilesPaths"));

    QStringList defaultNetbiosPaths;
    defaultNetbiosPaths << QStringLiteral("/var/log/samba/log.nmbd");
    mConfiguration->addItemStringList(QStringLiteral("NetbiosLogFilesPaths"), mNetbiosPaths,
                                      defaultNetbiosPaths, QStringLiteral("NetbiosLogFilesPaths"));
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
