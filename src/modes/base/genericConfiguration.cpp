/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "genericConfiguration.h"

#include <KLocalizedString>

#include "defaults.h"
#include "logging.h"

#include "globals.h"

#include "ksystemlogConfig.h"
GenericLogModeConfiguration::GenericLogModeConfiguration(const QString &configurationGroup,
                                                         const QStringList &defaultLogFilesPaths,
                                                         const QList<int> &defaultLogFilesLevels)
{
    logDebug() << "Using Configuration Group : " << configurationGroup;
    mConfiguration->setCurrentGroup(configurationGroup);

    mConfiguration->addItemStringList(QStringLiteral("LogFilesPaths"), mLogFilesPaths, defaultLogFilesPaths, QStringLiteral("LogFilesPaths"));

    mConfiguration->addItemIntList(QStringLiteral("LogFilesLevels"), mLogFilesLevels, defaultLogFilesLevels, QStringLiteral("LogFilesLevels"));
}

GenericLogModeConfiguration::~GenericLogModeConfiguration()
{
}

QStringList GenericLogModeConfiguration::logFilesPaths() const
{
    return mLogFilesPaths;
}

QList<int> GenericLogModeConfiguration::logFilesLevels() const
{
    return mLogFilesLevels;
}

void GenericLogModeConfiguration::setLogFilesPaths(const QStringList &logFilesPaths)
{
    mLogFilesPaths = logFilesPaths;
}

void GenericLogModeConfiguration::setLogFilesLevels(const QList<int> &logFilesLevels)
{
    mLogFilesLevels = logFilesLevels;
}

QVector<LogFile> GenericLogModeConfiguration::findGenericLogFiles() const
{
    QVector<LogFile> logFiles;

    if (mLogFilesPaths.size() != mLogFilesLevels.size()) {
        logDebug() << i18n("The two arrays size are different, skipping the reading of log files.");
        return logFiles;
    }

    LogLevel *level = nullptr;

    QListIterator<QString> itString(mLogFilesPaths);
    QListIterator<int> itInt(mLogFilesLevels);

    while (itString.hasNext()) {
        const int intValue = itInt.next();
        const QString stringValue = itString.next();

        level = Globals::instance().logLevels().value(Globals::LogLevelIds(intValue), Globals::instance().informationLogLevel());

        const QUrl url = QUrl::fromLocalFile(stringValue);
        if (!url.isValid()) {
            logWarning() << i18n("URL '%1' is not valid, skipping this URL.", url.path());
            continue;
        }

        logFiles.append(LogFile(url, level));
    }

    return logFiles;
}
