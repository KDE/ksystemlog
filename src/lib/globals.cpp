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

#include <QMap>

#include <KLocalizedString>

#include "defaults.h"
#include "logLevel.h"
#include "logFile.h"

#include "analyzer.h"
#include "logModeAction.h"
#include "logModeConfiguration.h"
#include "logModeConfigurationWidget.h"

#include "logging.h"

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
    QMap<QString, LogMode *> mLogModes;

    QList<LogModeAction *> mLogModeActions;

    QMetaEnum mLogLevelsMetaEnum;

    /**
     * Existing Log levels. The id value corresponds to the index in the vector
     */
    QList<LogLevel *> mLogLevels;

    QList<LogModeFactory *> mFactories;

    /**
     * These value are only pointers to item of the previous vector,
     * they are provided for convenience
     */
    LogLevel *mNoLogLevel = nullptr;
    LogLevel *mDebugLogLevel = nullptr;
    LogLevel *mInformationLogLevel = nullptr;
    LogLevel *mNoticeLogLevel = nullptr;
    LogLevel *mWarningLogLevel = nullptr;
    LogLevel *mErrorLogLevel = nullptr;
    LogLevel *mCriticalLogLevel = nullptr;
    LogLevel *mAlertLogLevel = nullptr;
    LogLevel *mEmergencyLogLevel = nullptr;
};

Globals::Globals()
    : d(new GlobalsPrivate())
{
    setupLogLevels();
}

Globals::~Globals()
{
    foreach (LogModeAction *logModeAction, d->mLogModeActions) {
        delete logModeAction;
    }
    d->mLogModeActions.clear();

    foreach (LogMode *logMode, d->mLogModes) {
        delete logMode;
    }
    d->mLogModes.clear();

    foreach (LogLevel *logLevel, d->mLogLevels) {
        delete logLevel;
    }
    d->mLogLevels.clear();

    foreach (LogModeFactory *factory, d->mFactories) {
        delete factory;
    }
    d->mFactories.clear();

    delete d;
}

QString Globals::formatDate(Globals::DateFormat format, const QDateTime &dateTime) const
{
    switch (format) {
    case LongFormat:
    case ShortFormat:
        return QLocale().toString(dateTime, static_cast<QLocale::FormatType>(format));
    case PreciseFormat:
        return dateTime.toString(QStringLiteral("dd.MM.yyyy hh:mm:ss:zzz"));
    }
    return QLocale().toString(dateTime, QLocale::ShortFormat);
}

void Globals::setupLogLevels()
{
    int index = metaObject()->indexOfEnumerator("LogLevelIds");
    d->mLogLevelsMetaEnum = metaObject()->enumerator(index);

    d->mNoLogLevel = new LogLevel(NONE_LOG_LEVEL_ID, i18n("None"), QStringLiteral("edit-none"),
                                  QColor(208, 210, 220));
    d->mLogLevels.append(d->mNoLogLevel);

    d->mDebugLogLevel
        = new LogLevel(DEBUG_LOG_LEVEL_ID, i18n("Debug"), QStringLiteral("debug-run"), QColor(156, 157, 165));
    d->mLogLevels.append(d->mDebugLogLevel);

    d->mInformationLogLevel
        = new LogLevel(INFORMATION_LOG_LEVEL_ID, i18n("Information"), QStringLiteral("dialog-information"),
                       QColor(36, 49, 103) /*QColor(0, 0, 0)*/);
    d->mLogLevels.append(d->mInformationLogLevel);

    d->mNoticeLogLevel
        = new LogLevel(NOTICE_LOG_LEVEL_ID, i18n("Notice"), QStringLiteral("note"), QColor(36, 138, 22));
    d->mLogLevels.append(d->mNoticeLogLevel);

    d->mWarningLogLevel = new LogLevel(WARNING_LOG_LEVEL_ID, i18n("Warning"), QStringLiteral("dialog-warning"),
                                       QColor(238, 144, 21));
    d->mLogLevels.append(d->mWarningLogLevel);

    d->mErrorLogLevel
        = new LogLevel(ERROR_LOG_LEVEL_ID, i18n("Error"), QStringLiteral("dialog-error"), QColor(173, 28, 28));
    d->mLogLevels.append(d->mErrorLogLevel);

    d->mCriticalLogLevel
        = new LogLevel(CRITICAL_LOG_LEVEL_ID, i18n("Critical"), QStringLiteral("dialog-error"), QColor(214, 26, 26));
    d->mLogLevels.append(d->mCriticalLogLevel);

    d->mAlertLogLevel
        = new LogLevel(ALERT_LOG_LEVEL_ID, i18n("Alert"), QStringLiteral("preferences-desktop-notification-bell"), QColor(214, 0, 0));
    d->mLogLevels.append(d->mAlertLogLevel);

    d->mEmergencyLogLevel = new LogLevel(EMERGENCY_LOG_LEVEL_ID, i18n("Emergency"),
                                         QStringLiteral("application-exit"), QColor(255, 0, 0));
    d->mLogLevels.append(d->mEmergencyLogLevel);
}

QList<LogMode *> Globals::logModes() const
{
    return d->mLogModes.values();
}

QList<LogLevel *> Globals::logLevels() const
{
    return d->mLogLevels;
}

LogLevel *Globals::noLogLevel() const
{
    return d->mNoLogLevel;
}

LogLevel *Globals::debugLogLevel() const
{
    return d->mDebugLogLevel;
}

LogLevel *Globals::informationLogLevel() const
{
    return d->mInformationLogLevel;
}

LogLevel *Globals::noticeLogLevel() const
{
    return d->mNoticeLogLevel;
}

LogLevel *Globals::warningLogLevel() const
{
    return d->mWarningLogLevel;
}

LogLevel *Globals::errorLogLevel() const
{
    return d->mErrorLogLevel;
}

LogLevel *Globals::criticalLogLevel() const
{
    return d->mCriticalLogLevel;
}

LogLevel *Globals::alertLogLevel() const
{
    return d->mAlertLogLevel;
}

LogLevel *Globals::emergencyLogLevel() const
{
    return d->mEmergencyLogLevel;
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
    return d->mLogLevelsMetaEnum;
}

void Globals::registerLogModeFactory(LogModeFactory *logModeFactory)
{
    const QList<LogMode *> logModes = logModeFactory->createLogModes();

    for (LogMode *logMode : logModes) {
        // Log mode
        d->mLogModes.insert(logMode->id(), logMode);
    }

    // Log mode Actions
    LogModeAction *logModeAction = logModeFactory->createLogModeAction();
    if (logModeAction) {
        d->mLogModeActions.append(logModeAction);
    }

    d->mFactories.append(logModeFactory);
}

LogMode *Globals::findLogMode(const QString &logModeName)
{
    return d->mLogModes.value(logModeName);
}

void Globals::recreateLogModeActions()
{
    // Delete existing log mode actions.
    foreach (LogModeAction *logModeAction, d->mLogModeActions) {
        delete logModeAction;
    }
    d->mLogModeActions.clear();

    // Create new log mode action for each log mode.
    foreach (LogModeFactory *factory, d->mFactories) {
        LogModeAction *logModeAction = factory->createLogModeAction();
        if (logModeAction) {
            d->mLogModeActions.append(logModeAction);
        }
    }
}

QList<LogModeAction *> Globals::logModeActions() const
{
    return d->mLogModeActions;
}
