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

#ifndef APACHE_CONFIGURATION_H
#define APACHE_CONFIGURATION_H

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "logging.h"

#include "apacheLogMode.h"

#include "ksystemlogConfig.h"

class ApacheConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    ApacheConfiguration();

    ~ApacheConfiguration() override;

    QStringList apachePaths() const;

    QStringList apacheAccessPaths() const;

    void setApachePaths(const QStringList &apachePaths);

    void setApacheAccessPaths(const QStringList &apacheAccessPaths);

private:
    QStringList mApachePaths;

    QStringList mApacheAccessPaths;
};

#endif // _APACHE_CONFIGURATION_H
