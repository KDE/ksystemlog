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

#ifndef _APACHE_CONFIGURATION_H_
#define _APACHE_CONFIGURATION_H_

#include <QStringList>

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include "apacheLogMode.h"

#include "ksystemlogConfig.h"

class ApacheConfigurationPrivate
{
public:
    QStringList apachePaths;

    QStringList apacheAccessPaths;
};

class ApacheConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    ApacheConfiguration()
        : d(new ApacheConfigurationPrivate())
    {
        configuration->setCurrentGroup(QStringLiteral("ApacheLogMode"));

        QStringList defaultApachePaths;
        defaultApachePaths << QStringLiteral("/var/log/apache2/error.log");
        configuration->addItemStringList(QStringLiteral("ApacheLogFilesPaths"), d->apachePaths,
                                         defaultApachePaths, QStringLiteral("ApacheLogFilesPaths"));

        QStringList defaultApacheAccessPaths;
        defaultApacheAccessPaths << QStringLiteral("/var/log/apache2/access.log");
        configuration->addItemStringList(QStringLiteral("ApacheAccessLogFilesPaths"), d->apacheAccessPaths,
                                         defaultApacheAccessPaths,
                                         QStringLiteral("ApacheAccessLogFilesPaths"));
    }

    virtual ~ApacheConfiguration() { delete d; }

    QStringList apachePaths() const { return d->apachePaths; }

    QStringList apacheAccessPaths() const { return d->apacheAccessPaths; }

    void setApachePaths(const QStringList &apachePaths) { d->apachePaths = apachePaths; }

    void setApacheAccessPaths(const QStringList &apacheAccessPaths)
    {
        d->apacheAccessPaths = apacheAccessPaths;
    }

private:
    ApacheConfigurationPrivate *const d;
};

#endif // _APACHE_CONFIGURATION_H_
