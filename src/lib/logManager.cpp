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

#include "logManager.h"

#include <KLocalizedString>
#include <KMessageBox>

#include "logging.h"
#include "analyzer.h"
#include "view.h"

#include "logViewWidget.h"

#include "loadingBar.h"

class LogManagerPrivate
{
    friend class LogManager;

    QTime lastUpdate;

    LogMode *logMode;

    View *usedView;
};

LogManager::LogManager(View *view)
    : d(new LogManagerPrivate())
{
    d->lastUpdate = QTime::currentTime();

    d->logMode = NULL;

    d->usedView = view;
    connect(d->usedView, SIGNAL(droppedUrls(QList<QUrl>)), this, SLOT(loadDroppedUrls(QList<QUrl>)));
}

LogManager::~LogManager()
{
    cleanPreviousLogMode();

    // usedView is managed by MainWindow
    // logMode is managed by Globals

    delete d;
}

View *LogManager::usedView() const
{
    return d->usedView;
}

void LogManager::reload()
{
    if (d->logMode == NULL) {
        logWarning() << "Log manager is not yet initialized";
        return;
    }

    logDebug() << "Reloading with log mode " << d->logMode->name() << "...";

    emit statusBarChanged(i18n("Loading log..."));

    // Change part of the main interface
    emit tabTitleChanged(d->usedView, d->logMode->icon(), d->logMode->name());
    emit windowTitleChanged(d->logMode->name());

    logDebug() << "Emptying view...";

    // Empty the current list, to better fill it
    d->usedView->logViewWidget()->model()->clear();

    logDebug() << "Initializing view...";

    // Init the Log View
    logDebug() << "Initializing columns view...";

    d->usedView->logViewWidget()->setColumns(d->logMode->analyzer()->initColumns());

    logDebug() << "Reading log...";

    // Read the log files
    d->logMode->analyzer()->watchLogFiles(false);
    d->logMode->analyzer()->watchLogFiles(true);

    emit statusBarChanged(i18n("Log successfully loaded."));

    // Log List has been totally reloaded
    emit reloaded();

    logDebug() << "Log mode " << d->logMode->name() << " reloaded";
}

void LogManager::stopWatching()
{
    d->logMode->analyzer()->watchLogFiles(false);
}

LogMode *LogManager::logMode()
{
    return d->logMode;
}

const QTime &LogManager::lastUpdate() const
{
    return d->lastUpdate;
}

void LogManager::updateLog(int lineCount)
{
    logDebug() << "Updating log " << lineCount << " new lines";

    if (lineCount == 0)
        return;

    d->lastUpdate = QTime::currentTime();

    emit logUpdated(d->usedView, lineCount);
}

void LogManager::cleanPreviousLogMode()
{
    logDebug() << "Cleaning previous LogMode...";

    d->logMode = NULL;
}

void LogManager::initialize(LogMode *mode)
{
    internalInitialize(mode, mode->createLogFiles());
}

void LogManager::internalInitialize(LogMode *mode, const QList<LogFile> &logFiles)
{
    logDebug() << "Initializing LogManager...";

    logDebug() << "Using files" << logFiles;

    cleanPreviousLogMode();

    // Use the new mode
    d->logMode = mode;

    // Find the Analyzer instance used for this new mode
    mode->analyzer()->setLogViewModel(d->usedView->logViewWidget()->model());

    mode->analyzer()->disconnect();

    connect(mode->analyzer(), SIGNAL(statusBarChanged(QString)), this, SIGNAL(statusBarChanged(QString)));
    connect(mode->analyzer(), SIGNAL(errorOccured(QString, QString)), this,
            SLOT(showErrorMessage(QString, QString)));
    connect(mode->analyzer(), SIGNAL(logUpdated(int)), this, SLOT(updateLog(int)));

    connect(mode->analyzer(), SIGNAL(readFileStarted(LogMode, LogFile, int, int)), d->usedView->loadingBar(),
            SLOT(startLoading(LogMode, LogFile, int, int)));
    connect(mode->analyzer(), SIGNAL(openingProgressed()), d->usedView->loadingBar(),
            SLOT(progressLoading()));
    connect(mode->analyzer(), SIGNAL(readEnded()), d->usedView->loadingBar(), SLOT(endLoading()));

    // Find the log files used for this kind of mode, and set them to our log manager
    mode->analyzer()->setLogFiles(logFiles);

    logDebug() << "LogManager initialized";
}

void LogManager::showErrorMessage(const QString &title, const QString &message)
{
    KMessageBox::error(d->usedView, message, title, KMessageBox::Notify);
}

void LogManager::setParsingPaused(bool paused)
{
    if (d->logMode == NULL) {
        logWarning() << "Log manager is not yet initialized";
        return;
    }

    d->logMode->analyzer()->setParsingPaused(paused);
}

bool LogManager::isParsingPaused() const
{
    if (d->logMode == NULL) {
        logWarning() << "Log manager is not yet initialized";
        return false;
    }

    return d->logMode->analyzer()->isParsingPaused();
}

void LogManager::loadDroppedUrls(const QList<QUrl> &urls)
{
    logDebug() << "Drop " << urls;

    QList<LogFile> logFiles;

    foreach (const QUrl &url, urls) {
        logFiles.append(LogFile(url, Globals::instance().informationLogLevel()));
    }

    if (logFiles.isEmpty() == false) {
        internalInitialize(Globals::instance().findLogMode(QLatin1String("openLogMode")), logFiles);

        reload();
    }
}
