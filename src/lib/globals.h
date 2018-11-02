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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QList>
#include <QString>
#include <QMetaEnum>

#include "logMode.h"
#include "logModeFactory.h"

class LogModeFactory;

class LogMode;
class LogModeAction;
class LogLevel;

class Reader;

class GlobalsPrivate;

class Globals : QObject
{
    Q_OBJECT

public:
    static Globals &instance();

    ~Globals();

    enum LogLevelIds {
        EMERGENCY_LOG_LEVEL_ID = 0,
        ALERT_LOG_LEVEL_ID,
        CRITICAL_LOG_LEVEL_ID,
        ERROR_LOG_LEVEL_ID,
        WARNING_LOG_LEVEL_ID,
        NOTICE_LOG_LEVEL_ID,
        INFORMATION_LOG_LEVEL_ID,
        DEBUG_LOG_LEVEL_ID,
        NONE_LOG_LEVEL_ID,

        LOG_LEVEL_NUM
    };
    Q_ENUM(LogLevelIds)

    enum DateFormat { LongFormat = 0, ShortFormat, PreciseFormat };

    QString formatDate(DateFormat format, const QDateTime &dateTime) const;

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

    QMetaEnum &logLevelsMetaEnum() const;

    /**
     * Allow to add a new Reader for a new log mode
     */
    void registerLogModeFactory(LogModeFactory *logModeFactory);

    QList<LogMode *> logModes();

    QList<LogModeAction *> logModeActions();

    LogMode *findLogMode(const QString &logModeName);

    void recreateLogModeActions();

private:
    explicit Globals();

    void setupLogLevels();

    GlobalsPrivate *const d;
};

#endif
