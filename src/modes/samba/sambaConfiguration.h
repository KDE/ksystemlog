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

#ifndef SAMBA_CONFIGURATION_H
#define SAMBA_CONFIGURATION_H

#include <QStringList>

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include "sambaLogMode.h"

#include "ksystemlogConfig.h"

class SambaConfigurationPrivate
{
public:
    QStringList sambaPaths;

    QStringList sambaAccessPaths;

    QStringList netbiosPaths;
};

class SambaConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    SambaConfiguration()
        : d(new SambaConfigurationPrivate())
    {
        configuration->setCurrentGroup(QStringLiteral("SambaLogMode"));

        QStringList defaultSambaPaths;
        defaultSambaPaths << QStringLiteral("/var/log/samba/log.smbd");
        configuration->addItemStringList(QStringLiteral("SambaLogFilesPaths"), d->sambaPaths,
                                         defaultSambaPaths, QStringLiteral("SambaLogFilesPaths"));

        QStringList defaultSambaAccessPaths;
        defaultSambaAccessPaths << QStringLiteral("/var/log/samba/log.localhost")
                                << QStringLiteral("/var/log/samba/log.127.0.0.1");
        configuration->addItemStringList(QStringLiteral("SambaAccessLogFilesPaths"), d->sambaAccessPaths,
                                         defaultSambaAccessPaths, QStringLiteral("SambaAccessLogFilesPaths"));

        QStringList defaultNetbiosPaths;
        defaultNetbiosPaths << QStringLiteral("/var/log/samba/log.nmbd");
        configuration->addItemStringList(QStringLiteral("NetbiosLogFilesPaths"), d->netbiosPaths,
                                         defaultNetbiosPaths, QStringLiteral("NetbiosLogFilesPaths"));
    }

    ~SambaConfiguration() override { delete d; }

    QStringList sambaPaths() const { return d->sambaPaths; }

    QStringList sambaAccessPaths() const { return d->sambaAccessPaths; }

    QStringList netbiosPaths() const { return d->netbiosPaths; }

    void setSambaPaths(const QStringList &sambaPaths) { d->sambaPaths = sambaPaths; }

    void setNetbiosPaths(const QStringList &netbiosPaths) { d->netbiosPaths = netbiosPaths; }

    void setSambaAccessPaths(const QStringList &sambaAccessPaths) { d->sambaAccessPaths = sambaAccessPaths; }

private:
    SambaConfigurationPrivate *const d;
};

#endif // _SAMBA_CONFIGURATION_H
