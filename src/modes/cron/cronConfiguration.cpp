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

#include "cronConfiguration.h"

CronConfiguration::CronConfiguration()
{
    configuration->setCurrentGroup(QStringLiteral("CronLogMode"));

    QStringList defaultCronPaths;
    defaultCronPaths << QStringLiteral("/var/log/syslog");
    configuration->addItemStringList(QStringLiteral("LogFilesPaths"), mCronPaths, defaultCronPaths,
                                     QStringLiteral("LogFilesPaths"));

    QString defaultProcessFilter(QStringLiteral("/usr/sbin/cron"));
    configuration->addItemString(QStringLiteral("ProcessFilter"), mProcessFilter, defaultProcessFilter,
                                 QStringLiteral("ProcessFilter"));
}

CronConfiguration::~CronConfiguration() {}

QString CronConfiguration::processFilter() const { return mProcessFilter; }

void CronConfiguration::setProcessFilter(const QString &processFilter) { mProcessFilter = processFilter; }

QStringList CronConfiguration::cronPaths() const { return mCronPaths; }

void CronConfiguration::setCronPaths(const QStringList &cronPaths) { mCronPaths = cronPaths; }
