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

#include <klocale.h>

#include <kmessagebox.h>

#include "logging.h"
#include "analyzer.h"
#include "view.h"

#include "logViewWidget.h"

#include "loadingBar.h"

class LogManagerPrivate {
	friend class LogManager;

	QTime lastUpdate;

	LogMode* logMode;

	Analyzer* analyzer;
	View* usedView;

};

LogManager::LogManager(View* view) :
	d(new LogManagerPrivate())
	{

	d->lastUpdate = QTime::currentTime();

	d->logMode = NULL;
	d->analyzer = NULL;

	d->usedView = view;
	connect(d->usedView, SIGNAL(droppedUrls(const KUrl::List&)), this, SLOT(loadDroppedUrls(const KUrl::List&)));
}

LogManager::~LogManager() {
	cleanPreviousLogMode();

	//usedView is managed by MainWindow
	//logMode is managed by Globals

	delete d;
}

View* LogManager::usedView() const {
	return d->usedView;
}

void LogManager::reload() {
	if (d->logMode == NULL) {
		logWarning() << "Log manager is not yet initialized" << endl;
		return;
	}

	logDebug() << "Reloading with log mode " << d->logMode->name() << "..." << endl;

	emit statusBarChanged(i18n("Loading log..."));

	// Change part of the main interface
	emit tabTitleChanged(d->usedView, d->logMode->icon(), d->logMode->name());
	emit windowTitleChanged(d->logMode->name());

	logDebug() << "Emptying view..." << endl;

	// Empty the current list, to better fill it
	d->usedView->logViewWidget()->model()->clear();

	logDebug() << "Initializing view..." << endl;

	// Init the Log View
	logDebug() << "Initializing columns view..." << endl;

	d->usedView->logViewWidget()->setColumns(d->analyzer->initColumns());

	logDebug() << "Reading log..." << endl;

	// Read the log files
	d->analyzer->watchLogFiles(true);

	emit statusBarChanged(i18n("Log successfully loaded."));

	// Log List has been totally reloaded
	emit reloaded();

	logDebug() << "Log mode " << d->logMode->name() << " reloaded" << endl;

}

LogMode* LogManager::logMode() {
	return d->logMode;
}

const QTime& LogManager::lastUpdate() const {
	return d->lastUpdate;
}

void LogManager::updateLog(int lineCount) {
	logDebug() << "Updating log " << lineCount  << " new lines" << endl;

	if (lineCount==0)
		return;

	d->lastUpdate = QTime::currentTime();

	emit logUpdated(d->usedView, lineCount);
}

void LogManager::cleanPreviousLogMode() {
	logDebug() << "Cleaning previous LogMode..." << endl;

	d->logMode = NULL;

	delete d->analyzer;
	d->analyzer = NULL;
}

void LogManager::initialize(LogMode* mode) {
	internalInitialize(mode, mode->createLogFiles());
}

void LogManager::internalInitialize(LogMode* mode, const QList<LogFile>& logFiles) {
	logDebug() << "Initializing LogManager..." << endl;

	logDebug() << "Using files" << logFiles << endl;

	cleanPreviousLogMode();

	//Use the new mode
	d->logMode=mode;

	//Find the Analyzer instance used for this new mode
	d->analyzer=mode->createAnalyzer();
	d->analyzer->setLogViewModel(d->usedView->logViewWidget()->model());

	connect(d->analyzer, SIGNAL(statusBarChanged(const QString&)), 				this, SIGNAL(statusBarChanged(const QString&)));
	connect(d->analyzer, SIGNAL(errorOccured(const QString&, const QString&)), 	this, SLOT(showErrorMessage(const QString&, const QString&)));
	connect(d->analyzer, SIGNAL(logUpdated(int)), 								this, SLOT(updateLog(int)));

	connect(d->analyzer, SIGNAL(readFileStarted(const LogMode&, const LogFile&, int, int)), 	d->usedView->loadingBar(), SLOT(startLoading(const LogMode&, const LogFile&, int, int)));
	connect(d->analyzer, SIGNAL(openingProgressed()), 	d->usedView->loadingBar(), SLOT(progressLoading()));
	connect(d->analyzer, SIGNAL(readEnded()), 			d->usedView->loadingBar(), SLOT(endLoading()));

	//Find the log files used for this kind of mode, and set them to our log manager
	d->analyzer->setLogFiles(logFiles);

	logDebug() << "LogManager initialized" << endl;

}

void LogManager::showErrorMessage(const QString& title, const QString& message) {
	KMessageBox::error(
		d->usedView,
		message,
		title,
		KMessageBox::Notify
	);
}

void LogManager::setParsingPaused(bool paused) {
	if (d->logMode == NULL) {
		logWarning() << "Log manager is not yet initialized" << endl;
		return;
	}

	d->analyzer->setParsingPaused(paused);
}

bool LogManager::isParsingPaused() const {
	if (d->logMode == NULL) {
		logWarning() << "Log manager is not yet initialized" << endl;
		return false;
	}

	return d->analyzer->isParsingPaused();
}

void LogManager::loadDroppedUrls(const KUrl::List& urls) {
	logDebug() << "Drop " << urls << endl;

	QList<LogFile> logFiles;

	foreach (const KUrl &url, urls) {
		logFiles.append(LogFile(url, Globals::instance()->informationLogLevel()));
	}

	if (logFiles.isEmpty() == false) {
		internalInitialize(Globals::instance()->findLogMode(QLatin1String( "openLogMode" )), logFiles);

		reload();
	}
}

#include "logManager.moc"
