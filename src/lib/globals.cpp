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

#include "globals.h"

#include <QString>
#include <QList>
#include <QMap>

#include <KLocalizedString>

#include "defaults.h"
#include "logLevel.h"
#include "logMode.h"
#include "logFile.h"

#include "analyzer.h"
#include "logModeAction.h"
#include "logModeFactory.h"
#include "logModeConfiguration.h"
#include "logModeConfigurationWidget.h"

#include "logging.h"

Q_LOGGING_CATEGORY(KSYSTEMLOG, "ksystemlog", QtWarningMsg)

Globals &Globals::instance()
{
    static Globals self;
    return self;
}

class GlobalsPrivate
{
public:
    /**
     * Existing Log modes.
     */
    QMap<QString, LogMode *> logModes;

    QList<LogModeAction *> logModeActions;

    QMetaEnum logLevelsMetaEnum;

    /**
     * Existing Log levels. The id value corresponds to the index in the vector
     */
    QList<LogLevel *> logLevels;

    QList<LogModeFactory *> factories;

    /**
     * These value are only pointers to item of the previous vector,
     * they are provided for convenience
     */
    LogLevel *noLogLevel;
    LogLevel *debugLogLevel;
    LogLevel *informationLogLevel;
    LogLevel *noticeLogLevel;
    LogLevel *warningLogLevel;
    LogLevel *errorLogLevel;
    LogLevel *criticalLogLevel;
    LogLevel *alertLogLevel;
    LogLevel *emergencyLogLevel;
};

Globals::Globals()
    : d(new GlobalsPrivate())
{
    setupLogLevels();
}

Globals::~Globals()
{
    foreach (LogModeAction *logModeAction, d->logModeActions) {
        delete logModeAction;
    }
    d->logModeActions.clear();

    foreach (LogMode *logMode, d->logModes) {
        delete logMode;
    }
    d->logModes.clear();

    foreach (LogLevel *logLevel, d->logLevels) {
        delete logLevel;
    }
    d->logLevels.clear();

    foreach (LogModeFactory *factory, d->factories) {
        delete factory;
    }
    d->factories.clear();

    delete d;
}

QString Globals::formatDate(Globals::DateFormat format, const QDateTime &dateTime) const
{
    switch (format) {
    case LongFormat:
    case ShortFormat:
        return QLocale().toString(dateTime, (QLocale::FormatType)format);
    case PreciseFormat:
        return dateTime.toString(QStringLiteral("dd.MM.yyyy hh:mm:ss:zzz"));
    default:
        break;
    }
    return QLocale().toString(dateTime, QLocale::ShortFormat);
}

void Globals::setupLogLevels()
{
    int index = metaObject()->indexOfEnumerator("LogLevelIds");
    d->logLevelsMetaEnum = metaObject()->enumerator(index);

    d->noLogLevel = new LogLevel(NONE_LOG_LEVEL_ID, i18n("None"), QStringLiteral("edit-none"),
                                 QColor(208, 210, 220));
    d->logLevels.append(d->noLogLevel);

    d->debugLogLevel
        = new LogLevel(DEBUG_LOG_LEVEL_ID, i18n("Debug"), QStringLiteral("debug-run"), QColor(156, 157, 165));
    d->logLevels.append(d->debugLogLevel);

    d->informationLogLevel
        = new LogLevel(INFORMATION_LOG_LEVEL_ID, i18n("Information"), QStringLiteral("dialog-information"),
                       QColor(36, 49, 103) /*QColor(0, 0, 0)*/);
    d->logLevels.append(d->informationLogLevel);

    d->noticeLogLevel
        = new LogLevel(NOTICE_LOG_LEVEL_ID, i18n("Notice"), QStringLiteral("note"), QColor(36, 138, 22));
    d->logLevels.append(d->noticeLogLevel);

    d->warningLogLevel = new LogLevel(WARNING_LOG_LEVEL_ID, i18n("Warning"), QStringLiteral("dialog-warning"),
                                      QColor(238, 144, 21));
    d->logLevels.append(d->warningLogLevel);

    d->errorLogLevel
        = new LogLevel(ERROR_LOG_LEVEL_ID, i18n("Error"), QStringLiteral("dialog-error"), QColor(173, 28, 28));
    d->logLevels.append(d->errorLogLevel);

    d->criticalLogLevel
        = new LogLevel(CRITICAL_LOG_LEVEL_ID, i18n("Critical"), QStringLiteral("dialog-error"), QColor(214, 26, 26));
    d->logLevels.append(d->criticalLogLevel);

    d->alertLogLevel
        = new LogLevel(ALERT_LOG_LEVEL_ID, i18n("Alert"), QStringLiteral("preferences-desktop-notification-bell"), QColor(214, 0, 0));
    d->logLevels.append(d->alertLogLevel);

    d->emergencyLogLevel = new LogLevel(EMERGENCY_LOG_LEVEL_ID, i18n("Emergency"),
                                        QStringLiteral("application-exit"), QColor(255, 0, 0));
    d->logLevels.append(d->emergencyLogLevel);
}

QList<LogMode *> Globals::logModes()
{
    return d->logModes.values();
}

QList<LogLevel *> Globals::logLevels()
{
    return d->logLevels;
}

LogLevel *Globals::noLogLevel()
{
    return d->noLogLevel;
}
LogLevel *Globals::debugLogLevel()
{
    return d->debugLogLevel;
}
LogLevel *Globals::informationLogLevel()
{
    return d->informationLogLevel;
}
LogLevel *Globals::noticeLogLevel()
{
    return d->noticeLogLevel;
}
LogLevel *Globals::warningLogLevel()
{
    return d->warningLogLevel;
}
LogLevel *Globals::errorLogLevel()
{
    return d->errorLogLevel;
}
LogLevel *Globals::criticalLogLevel()
{
    return d->criticalLogLevel;
}
LogLevel *Globals::alertLogLevel()
{
    return d->alertLogLevel;
}
LogLevel *Globals::emergencyLogLevel()
{
    return d->emergencyLogLevel;
}

LogLevel *Globals::logLevelByPriority(int id)
{
    switch (id) {
    case 0:
        return emergencyLogLevel();
    case 1:
        return alertLogLevel();
    case 2:
        return criticalLogLevel();
    case 3:
        return errorLogLevel();
    case 4:
        return warningLogLevel();
    case 5:
        return noticeLogLevel();
    case 6:
        return informationLogLevel();
    case 7:
        return debugLogLevel();
    }
    return noLogLevel();
}

QMetaEnum &Globals::logLevelsMetaEnum() const
{
    return d->logLevelsMetaEnum;
}

void Globals::registerLogModeFactory(LogModeFactory *logModeFactory)
{
    QList<LogMode *> logModes = logModeFactory->createLogModes();

    foreach (LogMode *logMode, logModes) {
        // Log mode
        d->logModes.insert(logMode->id(), logMode);
    }

    // Log mode Actions
    LogModeAction *logModeAction = logModeFactory->createLogModeAction();
    if (logModeAction != nullptr) {
        d->logModeActions.append(logModeAction);
    }

    d->factories.append(logModeFactory);
}

LogMode *Globals::findLogMode(const QString &logModeName)
{
    return d->logModes.value(logModeName);
}

void Globals::recreateLogModeActions()
{
    // Delete existing log mode actions.
    foreach (LogModeAction *logModeAction, d->logModeActions) {
        delete logModeAction;
    }
    d->logModeActions.clear();

    // Create new log mode action for each log mode.
    foreach (LogModeFactory *factory, d->factories) {
        LogModeAction *logModeAction = factory->createLogModeAction();
        if (logModeAction != nullptr) {
            d->logModeActions.append(logModeAction);
        }
    }
}

QList<LogModeAction *> Globals::logModeActions()
{
    return d->logModeActions;
}
