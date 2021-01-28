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

#ifndef DAEMON_LOG_MODE_H
#define DAEMON_LOG_MODE_H

/**
 * Daemon Log Mode Identifier
 */
#define DAEMON_LOG_MODE_ID "daemonLogMode"

/**
 * Daemon Log Icon
 */
#define DAEMON_MODE_ICON "utilities-terminal"

#include <QList>

#include "logMode.h"
#include "logging.h"

#include "daemonConfiguration.h"
#include "daemonConfigurationWidget.h"
#include "syslogAnalyzer.h"

#include "logModeItemBuilder.h"

class DaemonLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit DaemonLogMode();

    ~DaemonLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

#endif // _DAEMON_LOG_MODE_H
