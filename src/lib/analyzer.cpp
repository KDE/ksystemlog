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

#include "analyzer.h"

#include <klocale.h>

#include "logging.h"
#include "ksystemlogConfig.h"

#include "logViewModel.h"

#include "logMode.h"
#include "logFileReader.h"

#include "logFile.h"

Analyzer::Analyzer(LogMode* logMode) :
	QObject(NULL),
	logViewModel(NULL),
	logMode(logMode),
	logLineInternalIdGenerator(0) {
	
	parsingPaused = false;
	
	insertionLocking = new QMutex(QMutex::Recursive);
}

Analyzer::~Analyzer() {
	deleteLogFiles();
	
	//logMode is managed by Globals
	//logViewModel is managed by LogViewWidget
}

void Analyzer::watchLogFiles(bool enabled) {
	//Enable the log file watching, by revert order to read the most top file at last, and be sure its line will be kept

	QListIterator<LogFileReader*> it(logFileReaders);
	it.toBack();
	while (it.hasPrevious()) {
		LogFileReader* logFileReader = it.previous();
		logFileReader->watchFile(enabled);
	}
	
}

void Analyzer::deleteLogFiles() {
	watchLogFiles(false);
	
	// Remove the watching on the monitored files
	foreach (LogFileReader* logFileReader, logFileReaders) {
		logDebug() << "Remove file : " << logFileReader->logFile().url().path() << endl;
		delete logFileReader;
	}
	
	logFileReaders.clear();
}

bool Analyzer::isParsingPaused() const {
	return parsingPaused;
}

void Analyzer::setParsingPaused(bool paused) {
	parsingPaused = paused;

	bool watching;
	//If we resume the parsing, then parse files to know if new lines have been appended
	if (parsingPaused==true) {
		logDebug() << "Pausing reading" << endl;
		watching=false;
	}
	else {
		logDebug() << "Relaunch reading" << endl;
		watching=true;
	}
	
	watchLogFiles(watching);

}

void Analyzer::setLogViewModel(LogViewModel* logViewModel) {
	this->logViewModel = logViewModel;
}

void Analyzer::setLogFiles(const QList<LogFile>& logFiles) {
	//Remove previous files
	deleteLogFiles();
	
	foreach(const LogFile &logFile, logFiles) {
		LogFileReader* logFileReader = createLogFileReader(logFile);
		logFileReaders.append(logFileReader);
		
		connect(logFileReader, SIGNAL(contentChanged(LogFileReader*,Analyzer::ReadingMode,QStringList)), this, SLOT(logFileChanged(LogFileReader*,Analyzer::ReadingMode,QStringList)));
		connect(logFileReader, SIGNAL(statusBarChanged(QString)), this, SIGNAL(statusBarChanged(QString)));
		connect(logFileReader, SIGNAL(errorOccured(QString,QString)), this, SIGNAL(errorOccured(QString,QString)));
	}
}

void Analyzer::logFileChanged(LogFileReader* logFileReader, ReadingMode readingMode, const QStringList& content) {
	if (readingMode == Analyzer::FullRead)
		logDebug() << "File " << logFileReader->logFile().url().path() << " has been modified on full read." << endl;
	else
		logDebug() << "File " << logFileReader->logFile().url().path() << " has been modified on partial read" << endl;

	if (parsingPaused == true) {
		logDebug() << "Pause enabled. Nothing read." << endl;
		return;
	}

	logDebug() << "Locking file modifications of " << logFileReader->logFile().url().path() << endl;
	insertionLocking->lock();
	logDebug() << "Unlocking file modifications of " << logFileReader->logFile().url().path() << endl;

	QTime benchmark;
	benchmark.start();

	int insertedLogLineCount;
	
	logViewModel->startingMultipleInsertions(readingMode);
	
	if (readingMode == Analyzer::UpdatingRead) {
		insertedLogLineCount = insertLines(content, logFileReader->logFile(), Analyzer::UpdatingRead);
	}
	else {
		logDebug() << "Reading file "<< logFileReader->logFile().url().path() << endl;
		
		emit statusBarChanged( i18n("Opening '%1'...", logFileReader->logFile().url().path()) );
		
		//Inform that we are now reading the "index" file
		emit readFileStarted(*logMode, logFileReader->logFile(), logFileReaders.count() - logFileReaders.indexOf(logFileReader), logFileReaders.count());
		
		insertedLogLineCount = insertLines(content, logFileReader->logFile(), Analyzer::FullRead);

		emit statusBarChanged( i18n("Log file '%1' loaded successfully.", logFileReader->logFile().url().path()) );


	}	
	
	logViewModel->endingMultipleInsertions(readingMode, insertedLogLineCount);

	//Inform connected LoadingBar that the reading is now finished
	emit readEnded();

	//Inform LogManager that new lines have been added
	emit logUpdated(insertedLogLineCount);

	//Inform MainWindow status bar
	emit statusBarChanged( i18n("Log file '%1' has changed.", logFileReader->logFile().url().path()) );

	logDebug() << "Updating log files in " << benchmark.elapsed() << " ms" << endl;

	insertionLocking->unlock();
}

int Analyzer::insertLines(const QStringList& bufferedLines, const LogFile& logFile, ReadingMode readingMode) {
	logDebug() << "Inserting lines..." << endl;
	
	//If there is no line
	if (bufferedLines.size()==0) {
		logWarning() << "File is empty : " << logFile.url().path() << endl;
	}


	
	int stop = 0;
	int currentPosition = 0;
	
	QListIterator<QString> it(bufferedLines);
	/**
	 * If the log file is sorted, then we can ignore the first lines 
	 * if there are more lines in the log file than the max lines
	 * 
	 * TODO Read revertly the file and ignore last lines if we are in Descending mode
	 */
	logDebug() << "Log file Sort mode is " << logFileSortMode() << endl;
	if (logFileSortMode() == Analyzer::AscendingSortedLogFile) {
		//Calculate how many lines we will ignore
		if (bufferedLines.size() > KSystemLogConfig::maxLines()) {
			stop=bufferedLines.size() - KSystemLogConfig::maxLines();
		}

		//Ignore those lines
		while (currentPosition < stop) {
			it.next();
			++currentPosition;
		}
	}
	
	int insertedLogLineCount = 0;
	while (currentPosition < bufferedLines.size()) {
		QString buffer(it.next());
 
		bool inserted = insertLine(buffer, logFile, readingMode);
		if (inserted) {
			insertedLogLineCount++;
		}
		
		if (readingMode == Analyzer::FullRead) {
			informOpeningProgress(currentPosition, (bufferedLines.size()-1) - stop);
		}

		++currentPosition;
	}

	logDebug() << "Total read lines :"<< (bufferedLines.size() - stop) << "(" << logFile.url().path() << ")" << endl;
	
	return insertedLogLineCount;
}

bool Analyzer::insertLine(const QString& buffer, const LogFile& originalFile, ReadingMode readingMode) {
	LogLine* line=parseMessage(buffer, originalFile);

	//Invalid log line
	if (line==NULL) {
		return false;
	}
	
	//On full reading, it is not needed to display the recent status
	if (readingMode == Analyzer::FullRead) {
		line->setRecent(false);
	}
	
	return logViewModel->insertNewLogLine(line);
}

inline void Analyzer::informOpeningProgress(int currentPosition, int total) {
	int each=total / 100;
	if (each==0) {
		return;
	}

	if (currentPosition%each==0) {
		emit openingProgressed();
	}
}


