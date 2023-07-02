/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "globals.h"

#include <QMap>

#include <KLocalizedString>

#include "defaults.h"
#include "logFile.h"
#include "logLevel.h"

#include "analyzer.h"
#include "logModeAction.h"
#include "logModeConfiguration.h"
#include "logModeConfigurationWidget.h"

#include "ksystemlog_debug.h"

#include <KColorScheme>

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
    QHash<Globals::LogLevelIds, LogLevel *> mLogLevels;

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
    qDeleteAll(d->mLogModeActions);
    d->mLogModeActions.clear();

    qDeleteAll(d->mLogModes);
    d->mLogModes.clear();

    qDeleteAll(d->mLogLevels);
    d->mLogLevels.clear();

    qDeleteAll(d->mFactories);
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
    KColorScheme scheme(QPalette::Active, KColorScheme::View);

    QColor redColor = scheme.foreground(KColorScheme::NegativeText).color();
    QColor inactiveText = scheme.foreground(KColorScheme::InactiveText).color();
    int index = metaObject()->indexOfEnumerator("LogLevelIds");
    d->mLogLevelsMetaEnum = metaObject()->enumerator(index);

    d->mNoLogLevel = new LogLevel(NONE_LOG_LEVEL_ID, i18n("None"), QStringLiteral("edit-none"), inactiveText.darker(120));
    d->mLogLevels[NONE_LOG_LEVEL_ID] = d->mNoLogLevel;

    d->mDebugLogLevel = new LogLevel(DEBUG_LOG_LEVEL_ID, i18n("Debug"), QStringLiteral("debug-run"), inactiveText);
    d->mLogLevels[DEBUG_LOG_LEVEL_ID] = d->mDebugLogLevel;

    d->mInformationLogLevel = new LogLevel(INFORMATION_LOG_LEVEL_ID,
                                           i18n("Information"),
                                           QStringLiteral("dialog-information"),
                                           scheme.foreground(KColorScheme::ActiveText).color() /*QColor(0, 0, 0)*/);
    d->mLogLevels[INFORMATION_LOG_LEVEL_ID] = d->mInformationLogLevel;

    d->mNoticeLogLevel = new LogLevel(NOTICE_LOG_LEVEL_ID, i18n("Notice"), QStringLiteral("note"), scheme.foreground(KColorScheme::PositiveText).color());
    d->mLogLevels[NOTICE_LOG_LEVEL_ID] = d->mNoticeLogLevel;

    d->mWarningLogLevel =
        new LogLevel(WARNING_LOG_LEVEL_ID, i18n("Warning"), QStringLiteral("dialog-warning"), scheme.foreground(KColorScheme::NeutralText).color());
    d->mLogLevels[WARNING_LOG_LEVEL_ID] = d->mWarningLogLevel;

    d->mErrorLogLevel = new LogLevel(ERROR_LOG_LEVEL_ID, i18n("Error"), QStringLiteral("dialog-error"), redColor.darker(200));
    d->mLogLevels[ERROR_LOG_LEVEL_ID] = d->mErrorLogLevel;

    d->mCriticalLogLevel = new LogLevel(CRITICAL_LOG_LEVEL_ID, i18n("Critical"), QStringLiteral("dialog-error"), redColor.darker(150));
    d->mLogLevels[CRITICAL_LOG_LEVEL_ID] = d->mCriticalLogLevel;

    d->mAlertLogLevel = new LogLevel(ALERT_LOG_LEVEL_ID, i18n("Alert"), QStringLiteral("preferences-desktop-notification-bell"), redColor.darker(120));
    d->mLogLevels[ALERT_LOG_LEVEL_ID] = d->mAlertLogLevel;

    d->mEmergencyLogLevel = new LogLevel(EMERGENCY_LOG_LEVEL_ID, i18n("Emergency"), QStringLiteral("application-exit"), redColor);
    d->mLogLevels[EMERGENCY_LOG_LEVEL_ID] = d->mEmergencyLogLevel;
}

QList<LogMode *> Globals::logModes() const
{
    return d->mLogModes.values();
}

QHash<Globals::LogLevelIds, LogLevel *> Globals::logLevels() const
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
    qDeleteAll(d->mLogModeActions);
    d->mLogModeActions.clear();

    // Create new log mode action for each log mode.
    for (LogModeFactory *factory : std::as_const(d->mFactories)) {
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

#include "moc_globals.cpp"
