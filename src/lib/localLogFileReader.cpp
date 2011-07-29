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

#include "localLogFileReader.h"

#include <QMutex>
#include <QFile>

#include <kdirwatch.h>
#include <klocale.h>
#include <kfilterdev.h>
#include <kmimetype.h>
#include <kurl.h>

#include "logFileReader.h"
#include "logFileReaderPrivate.h"

#include "logging.h"


class LocalLogFileReaderPrivate : public LogFileReaderPrivate {

public:

	KDirWatch* watch;

	long previousFilePosition;

	/**
	 * Mutex avoiding multiple logFileModified() calls
	 */
	QMutex insertionLocking;

};


LocalLogFileReader::LocalLogFileReader(const LogFile& logFile) :
	LogFileReader(*new LocalLogFileReaderPrivate(), logFile) {

	init();
}


LocalLogFileReader::LocalLogFileReader(LocalLogFileReaderPrivate& dd, const LogFile& logFile) :
	LogFileReader(dd, logFile) {

	init();
}

LocalLogFileReader::~LocalLogFileReader() {
	Q_D(LocalLogFileReader);

	//Delete the watching object
	delete d->watch;

	//d pointer is deleted by the parent class
}

void LocalLogFileReader::init() {
	Q_D(LocalLogFileReader);

	d->watch=new KDirWatch();
	connect(d->watch, SIGNAL(dirty(QString)), this, SLOT(logFileModified()));

	//Init current file position
	d->previousFilePosition = 0;

	logDebug() << "Reading local file " << d->logFile.url().path() << endl;

}

void LocalLogFileReader::watchFile(bool enable) {
	Q_D(LocalLogFileReader);

	if (enable == true) {
		logDebug() << "Monitoring file : " << d->logFile.url().path() << endl;

		if (d->watch->contains(d->logFile.url().path()) == false) {
			d->watch->addFile(d->logFile.url().path());
		}

		//Reinit current file position
		d->previousFilePosition = 0;

		//If we enable the watching, then we first try to see if new lines have appeared
		logFileModified();
	}
	else {
		d->watch->removeFile(d->logFile.url().path());
	}
}

QIODevice* LocalLogFileReader::open() {
	Q_D(LocalLogFileReader);

	if (d->logFile.url().isValid()==false) {
		QString message(i18n("This file is not valid. Please adjust it in the settings of KSystemLog."));
		emit errorOccured(i18n("File Does Not Exist"), message);
		emit statusBarChanged(message);
	}

	QString mimeType = KMimeType::findByFileContent( d->logFile.url().path() )->name();

	logDebug() << d->logFile.url().path() << " : " << mimeType << endl;
	QIODevice* inputDevice;

	//Try to see if this file exists
	QFile* file = new QFile(d->logFile.url().path());
	//If the file does not exist
	if (! file->exists()) {
		QString message(i18n("The file '%1' does not exist.", d->logFile.url().path()));
		emit errorOccured(i18n("File Does Not Exist"), message);
		emit statusBarChanged(message);
		delete file;
		return NULL;
	}

	//Plain text file : we use a QFile object
	if (mimeType == QLatin1String( "text/plain" ) || mimeType == QLatin1String( "application/octet-stream" )) {
		logDebug() << "Using QFile input device" << endl;

		inputDevice = file;
	}
	//Compressed file : we use the KFilterDev helper
	else {
		logDebug() << "Using KFilterDev input device" << endl;

		inputDevice = KFilterDev::deviceForFile(d->logFile.url().path(), mimeType);

		if (inputDevice == NULL) {
			QString message(i18n("Unable to uncompress the '%2' format of '%1'.", d->logFile.url().path(), mimeType));
			emit errorOccured(i18n("Unable to Uncompress File"), message);
			emit statusBarChanged(message);
			return NULL;
		}
	}

	if ( ! inputDevice->open( QIODevice::ReadOnly ) ) {
		QString message(i18n("You do not have sufficient permissions to read '%1'.", d->logFile.url().path()));
		emit errorOccured(i18n("Insufficient Permissions"), message);
		emit statusBarChanged(message);
		delete inputDevice;
		return NULL;
	}

	return inputDevice;
}

void LocalLogFileReader::close(QIODevice* inputDevice) {
	inputDevice->close();
	delete inputDevice;
}

QStringList LocalLogFileReader::readContent(QIODevice* inputDevice) {
	logDebug() << "Retrieving raw buffer..."<< endl;

	Q_D(LocalLogFileReader);

	QStringList rawBuffer;

	QTextStream inputStream(inputDevice);
	while (inputStream.atEnd() == false) {
		rawBuffer.append(inputStream.readLine());
	}

	logDebug() << "Raw buffer retrieved."<< endl;

	//Get the size file for the next calculation
	d->previousFilePosition = inputDevice->size();
	logDebug() << "New file position : " << d->previousFilePosition << " (" << d->logFile.url().path() << ")" <<  endl;

	return rawBuffer;
}

void LocalLogFileReader::logFileModified() {
	Q_D(LocalLogFileReader);

	logDebug() << "Locking log file modification..." << endl;
	if (d->insertionLocking.tryLock() == false) {
		logDebug() << "Log file modification already detected." << endl;
		return;
	}

	QIODevice* inputDevice = open();
	if (inputDevice == NULL) {
		logError() << "Could not open file " << d->logFile.url().path() << endl;
		return;
	}

	//If there are new lines in the file, insert only them or this is the first time we read this file
	if (d->previousFilePosition!=0 && d->previousFilePosition <= inputDevice->size()) {
		logDebug() << "Reading from position " << d->previousFilePosition << " (" << d->logFile.url().path() << ")" << endl;

		if (inputDevice->isSequential()) {
			logError() << "The file current position could not be modified" << endl;
		}
		else {
			//Place the cursor to the last line opened
			inputDevice->seek(d->previousFilePosition);
		}

		logDebug() << "Retrieving a part of the file..."<< endl;

		emit contentChanged(this, Analyzer::UpdatingRead, readContent(inputDevice));

	}
	//Else reread all lines, clear log list
	else {
		logDebug() << "New file or file truncated. (Re-)Loading log file" << endl;

		emit contentChanged(this, Analyzer::FullRead, readContent(inputDevice));

	}

	close(inputDevice);

	logDebug() << "Unlocking log file modification..." << endl;
	d->insertionLocking.unlock();
}

#include "localLogFileReader.moc"
