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

#ifndef _DAEMON_CONFIGURATION_H_
#define _DAEMON_CONFIGURATION_H_

#include <QStringList>

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include "ksystemlogConfig.h"

class DaemonConfigurationPrivate
{
public:
    QStringList daemonPaths;
};

class DaemonConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    DaemonConfiguration()
        : d(new DaemonConfigurationPrivate())
    {
        configuration->setCurrentGroup(QStringLiteral("DaemonLogMode"));

        QStringList defaultDaemonPaths;
        defaultDaemonPaths << QStringLiteral("/var/log/daemon.log");
        configuration->addItemStringList(QStringLiteral("LogFilesPaths"), d->daemonPaths, defaultDaemonPaths,
                                         QStringLiteral("LogFilesPaths"));
    }

    virtual ~DaemonConfiguration() { delete d; }

    QStringList daemonPaths() const { return d->daemonPaths; }

    void setDaemonPaths(const QStringList &daemonPaths) { d->daemonPaths = daemonPaths; }

private:
    DaemonConfigurationPrivate *const d;
};

#endif // _DAEMON_CONFIGURATION_H_
