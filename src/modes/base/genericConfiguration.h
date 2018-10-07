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

#ifndef _LOG_MODE_GENERIC_CONFIGURATION_H_
#define _LOG_MODE_GENERIC_CONFIGURATION_H_

#include <QStringList>
#include <QList>

#include "logFile.h"

#include "logModeConfiguration.h"

class GenericLogModeConfigurationPrivate;

class GenericLogModeConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    GenericLogModeConfiguration(const QString &configurationGroup, const QStringList &defaultLogFilesPaths,
                                const QList<int> &defaultLogFilesLevels);
    virtual ~GenericLogModeConfiguration();

    QStringList logFilesPaths() const;

    QList<int> logFilesLevels() const;

    void setLogFilesPaths(const QStringList &logFilesPaths);

    void setLogFilesLevels(const QList<int> &logFilesLevels);

    QList<LogFile> findGenericLogFiles();

private:
    GenericLogModeConfigurationPrivate *const d;
};

#endif // _LOG_MODE_GENERIC_CONFIGURATION_H_
