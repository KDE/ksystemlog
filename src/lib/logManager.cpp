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

    LogMode *logMode = nullptr;

    Analyzer *analyzer = nullptr;
    View *usedView = nullptr;
    QString analyzerStatus;
    QVariant analyzerOptions;
};

LogManager::LogManager(View *view)
    : d(new LogManagerPrivate())
{
    d->lastUpdate = QTime::currentTime();

    d->logMode = nullptr;
    d->analyzer = nullptr;

    d->usedView = view;
    connect(d->usedView, &View::droppedUrls, this, &LogManager::loadDroppedUrls);
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
    if (!d->logMode) {
        logWarning() << "Log manager is not yet initialized";
        return;
    }

    logDebug() << "Reloading with log mode " << d->logMode->name() << "...";

    Q_EMIT statusBarChanged(i18n("Loading log..."));

    // Change part of the main interface
    Q_EMIT tabTitleChanged(d->usedView, d->logMode->icon(), d->logMode->name());
    Q_EMIT windowTitleChanged(d->logMode->name());

    logDebug() << "Emptying view...";

    // Empty the current list, to better fill it
    d->usedView->logViewWidget()->model()->clear();

    logDebug() << "Initializing view...";

    // Init the Log View
    logDebug() << "Initializing columns view...";

    d->usedView->logViewWidget()->setColumns(d->analyzer->initColumns());

    logDebug() << "Reading log...";

    // Read the log files
    d->analyzer->watchLogFiles(false);
    d->analyzer->watchLogFiles(true);

    Q_EMIT statusBarChanged(i18n("Log successfully loaded."));

    // Log List has been totally reloaded
    Q_EMIT reloaded();

    logDebug() << "Log mode " << d->logMode->name() << " reloaded";
}

void LogManager::stopWatching()
{
    if (d->analyzer)
        d->analyzer->watchLogFiles(false);
}

const QVariant &LogManager::analyzerOptions() const
{
    return d->analyzerOptions;
}

LogMode *LogManager::logMode()
{
    return d->logMode;
}

QString LogManager::title() const
{
    if (!d->analyzerStatus.isEmpty())
        return d->logMode->name() + QStringLiteral(" - ") + d->analyzerStatus;
    else
        return d->logMode->name();
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

    Q_EMIT logUpdated(d->usedView, lineCount);
}

void LogManager::cleanPreviousLogMode()
{
    logDebug() << "Cleaning previous LogMode...";

    d->logMode = nullptr;

    delete d->analyzer;
    d->analyzer = nullptr;

    d->analyzerStatus.clear();
}

void LogManager::initialize(LogMode *mode, const QVariant &analyzerOptions)
{
    internalInitialize(mode, mode->createLogFiles(), analyzerOptions);
}

void LogManager::internalInitialize(LogMode *mode, const QList<LogFile> &logFiles,
                                    const QVariant &analyzerOptions)
{
    logDebug() << "Initializing LogManager...";

    logDebug() << "Using files" << logFiles;

    cleanPreviousLogMode();

    d->analyzerOptions = analyzerOptions;

    // Use the new mode
    d->logMode = mode;

    // Find the Analyzer instance used for this new mode
    d->analyzer = mode->createAnalyzer(analyzerOptions);
    d->analyzer->setLogViewModel(d->usedView->logViewWidget()->model());
    connect(d->analyzer, &Analyzer::statusChanged, this, [this](const QString &status) {
        d->analyzerStatus = status;
        Q_EMIT tabTitleChanged(d->usedView, d->logMode->icon(), title());
        Q_EMIT windowTitleChanged(title());
    });

    connect(d->analyzer, &Analyzer::statusBarChanged, this, &LogManager::statusBarChanged);
    connect(d->analyzer, &Analyzer::errorOccured, this,
            &LogManager::showErrorMessage);
    connect(d->analyzer, &Analyzer::logUpdated, this, &LogManager::updateLog);

    connect(d->analyzer, &Analyzer::readFileStarted, d->usedView->loadingBar(),
            &LoadingBar::startLoading);
    connect(d->analyzer, &Analyzer::openingProgressed, d->usedView->loadingBar(), &LoadingBar::progressLoading);
    connect(d->analyzer, &Analyzer::readEnded, d->usedView->loadingBar(), &LoadingBar::endLoading);

    // Find the log files used for this kind of mode, and set them to our log manager
    d->analyzer->setLogFiles(logFiles);

    logDebug() << "LogManager initialized";
}

void LogManager::showErrorMessage(const QString &title, const QString &message)
{
    KMessageBox::error(d->usedView, message, title, KMessageBox::Notify);
}

void LogManager::setParsingPaused(bool paused)
{
    if (d->logMode == nullptr) {
        logWarning() << "Log manager is not yet initialized";
        return;
    }

    if (!paused) {
        // Current analyzer implementations just perform full reload when resuming.
        // Clear the log view to avoid duplicating entries.
        d->usedView->logViewWidget()->model()->clear();
        d->usedView->logViewWidget()->setColumns(d->analyzer->initColumns());
    }
    d->analyzer->setParsingPaused(paused);
}

bool LogManager::isParsingPaused() const
{
    if (d->logMode == nullptr) {
        logWarning() << "Log manager is not yet initialized";
        return false;
    }

    return d->analyzer->isParsingPaused();
}

void LogManager::loadDroppedUrls(const QList<QUrl> &urls)
{
    logDebug() << "Drop " << urls;

    QList<LogFile> logFiles;
    logFiles.reserve(urls.count());

    for (const QUrl &url : urls) {
        logFiles.append(LogFile(url, Globals::instance().informationLogLevel()));
    }

    if (logFiles.isEmpty() == false) {
        internalInitialize(Globals::instance().findLogMode(QStringLiteral("openLogMode")), logFiles);

        reload();
    }
}
