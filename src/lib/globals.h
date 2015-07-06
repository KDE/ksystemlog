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

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <QList>
#include <QString>

#include "logMode.h"
#include "logModeFactory.h"

class LogModeFactory;

class LogMode;
class LogModeAction;
class LogLevel;

class Reader;

class GlobalsPrivate;

class Globals
{
public:
    static Globals &instance();

    ~Globals();

    QList<LogLevel *> logLevels();

    LogLevel *noLogLevel();
    LogLevel *debugLogLevel();
    LogLevel *informationLogLevel();
    LogLevel *noticeLogLevel();
    LogLevel *warningLogLevel();
    LogLevel *errorLogLevel();
    LogLevel *criticalLogLevel();
    LogLevel *alertLogLevel();
    LogLevel *emergencyLogLevel();

    LogLevel *logLevelByPriority(int id);

    /**
     * Allow to add a new Reader for a new log mode
     */
    void registerLogModeFactory(LogModeFactory *logModeFactory);

    QList<LogMode *> logModes();

    QList<LogModeAction *> logModeActions();

    LogMode *findLogMode(const QString &logModeName);

private:
    explicit Globals();

    void setupLogLevels();

    GlobalsPrivate *const d;
};

#endif
