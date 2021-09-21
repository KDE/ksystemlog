/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logManager.h"

#include <KLocalizedString>
#include <KMessageBox>

#include "analyzer.h"
#include "ksystemlog_debug.h"
#include "view.h"

#include "logViewWidget.h"

#include "loadingBar.h"

class LogManagerPrivate
{
    friend class LogManager;

    QTime mLastUpdate;

    LogMode *mLogMode = nullptr;

    Analyzer *mAnalyzer = nullptr;
    View *mUsedView = nullptr;
    QString mAnalyzerStatus;
    QVariant mAnalyzerOptions;
};

LogManager::LogManager(View *view)
    : QObject(view)
    , d(new LogManagerPrivate())
{
    d->mLastUpdate = QTime::currentTime();

    d->mUsedView = view;
    connect(d->mUsedView, &View::droppedUrls, this, &LogManager::loadDroppedUrls);
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
    return d->mUsedView;
}

void LogManager::reload()
{
    if (!d->mLogMode) {
        qCWarning(KSYSTEMLOG) << "Log manager is not yet initialized";
        return;
    }

    qCDebug(KSYSTEMLOG) << "Reloading with log mode " << d->mLogMode->name() << "...";

    Q_EMIT statusBarChanged(i18n("Loading log..."));

    // Change part of the main interface
    Q_EMIT tabTitleChanged(d->mUsedView, d->mLogMode->icon(), d->mLogMode->name());
    Q_EMIT windowTitleChanged(d->mLogMode->name());

    qCDebug(KSYSTEMLOG) << "Emptying view...";

    // Empty the current list, to better fill it
    d->mUsedView->logViewWidget()->model()->clear();

    qCDebug(KSYSTEMLOG) << "Initializing view...";

    // Init the Log View
    qCDebug(KSYSTEMLOG) << "Initializing columns view...";

    d->mUsedView->logViewWidget()->setColumns(d->mAnalyzer->initColumns());

    qCDebug(KSYSTEMLOG) << "Reading log...";

    // Read the log files
    d->mAnalyzer->watchLogFiles(false);
    d->mAnalyzer->watchLogFiles(true);

    Q_EMIT statusBarChanged(i18n("Log successfully loaded."));

    // Log List has been totally reloaded
    Q_EMIT reloaded();

    qCDebug(KSYSTEMLOG) << "Log mode " << d->mLogMode->name() << " reloaded";
}

void LogManager::stopWatching()
{
    if (d->mAnalyzer) {
        d->mAnalyzer->watchLogFiles(false);
    }
}

const QVariant &LogManager::analyzerOptions() const
{
    return d->mAnalyzerOptions;
}

LogMode *LogManager::logMode()
{
    return d->mLogMode;
}

QString LogManager::title() const
{
    if (!d->mAnalyzerStatus.isEmpty()) {
        return d->mLogMode->name() + QStringLiteral(" - ") + d->mAnalyzerStatus;
    } else {
        return d->mLogMode->name();
    }
}

const QTime &LogManager::lastUpdate() const
{
    return d->mLastUpdate;
}

void LogManager::updateLog(int lineCount)
{
    qCDebug(KSYSTEMLOG) << "Updating log " << lineCount << " new lines";

    if (lineCount == 0) {
        return;
    }

    d->mLastUpdate = QTime::currentTime();

    Q_EMIT logUpdated(d->mUsedView, lineCount);
}

void LogManager::cleanPreviousLogMode()
{
    qCDebug(KSYSTEMLOG) << "Cleaning previous LogMode...";

    d->mLogMode = nullptr;

    delete d->mAnalyzer;
    d->mAnalyzer = nullptr;

    d->mAnalyzerStatus.clear();
}

void LogManager::initialize(LogMode *mode, const QVariant &analyzerOptions)
{
    if (!mode) {
        return;
    }
    internalInitialize(mode, mode->createLogFiles(), analyzerOptions);
}

void LogManager::internalInitialize(LogMode *mode, const QVector<LogFile> &logFiles, const QVariant &analyzerOptions)
{
    qCDebug(KSYSTEMLOG) << "Initializing LogManager...";

    qCDebug(KSYSTEMLOG) << "Using files" << logFiles;

    cleanPreviousLogMode();

    d->mAnalyzerOptions = analyzerOptions;

    // Use the new mode
    d->mLogMode = mode;

    // Find the Analyzer instance used for this new mode
    d->mAnalyzer = mode->createAnalyzer(analyzerOptions);
    d->mAnalyzer->setLogViewModel(d->mUsedView->logViewWidget()->model());
    connect(d->mAnalyzer, &Analyzer::statusChanged, this, [this](const QString &status) {
        d->mAnalyzerStatus = status;
        Q_EMIT tabTitleChanged(d->mUsedView, d->mLogMode->icon(), title());
        Q_EMIT windowTitleChanged(title());
    });

    connect(d->mAnalyzer, &Analyzer::statusBarChanged, this, &LogManager::statusBarChanged);
    connect(d->mAnalyzer, &Analyzer::errorOccured, this, &LogManager::showErrorMessage);
    connect(d->mAnalyzer, &Analyzer::logUpdated, this, &LogManager::updateLog);

    connect(d->mAnalyzer, &Analyzer::readFileStarted, d->mUsedView->loadingBar(), &LoadingBar::startLoading);
    connect(d->mAnalyzer, &Analyzer::openingProgressed, d->mUsedView->loadingBar(), &LoadingBar::progressLoading);
    connect(d->mAnalyzer, &Analyzer::readEnded, d->mUsedView->loadingBar(), &LoadingBar::endLoading);

    // Find the log files used for this kind of mode, and set them to our log manager
    d->mAnalyzer->setLogFiles(logFiles);

    qCDebug(KSYSTEMLOG) << "LogManager initialized";
}

void LogManager::showErrorMessage(const QString &title, const QString &message)
{
    KMessageBox::error(d->mUsedView, message, title, KMessageBox::Notify);
}

void LogManager::setParsingPaused(bool paused)
{
    if (!d->mLogMode) {
        qCWarning(KSYSTEMLOG) << "Log manager is not yet initialized";
        return;
    }

    if (!paused) {
        // Current analyzer implementations just perform full reload when resuming.
        // Clear the log view to avoid duplicating entries.
        d->mUsedView->logViewWidget()->model()->clear();
        d->mUsedView->logViewWidget()->setColumns(d->mAnalyzer->initColumns());
    }
    d->mAnalyzer->setParsingPaused(paused);
}

bool LogManager::isParsingPaused() const
{
    if (!d->mLogMode) {
        qCWarning(KSYSTEMLOG) << "Log manager is not yet initialized";
        return false;
    }

    return d->mAnalyzer->isParsingPaused();
}

void LogManager::loadDroppedUrls(const QList<QUrl> &urls)
{
    qCDebug(KSYSTEMLOG) << "Drop " << urls;

    QVector<LogFile> logFiles;
    logFiles.reserve(urls.count());

    for (const QUrl &url : urls) {
        logFiles.append(LogFile(url, Globals::instance().informationLogLevel()));
    }

    if (!logFiles.isEmpty()) {
        internalInitialize(Globals::instance().findLogMode(QStringLiteral("openLogMode")), logFiles);

        reload();
    }
}
