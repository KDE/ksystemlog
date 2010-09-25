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

#include "kioLogFileReader.h"

#include <QIODevice>

#include <kio/filejob.h>
#include <kio/job.h>

#include <kdirwatch.h>

#include "logFile.h"
#include "logging.h"

#define READ_SIZE 10

class KioLogFileReaderPrivate {

public:

	KioLogFileReaderPrivate(const LogFile& file) :
		logFile(file) {

	}

	LogFile logFile;

	KIO::FileJob* fileJob;

	QString buffer;
	qulonglong totalRead;

	KDirWatch* fileWatch;

};

KioLogFileReader::KioLogFileReader(const LogFile& logFile) :
	d(new KioLogFileReaderPrivate(logFile)) {

	d->fileJob = NULL;
	d->totalRead = 0;

	d->fileWatch = new KDirWatch(this);

	connect(d->fileWatch, SIGNAL(dirty(const QString&)), this, SLOT(watchFile(const QString&)));
	d->fileWatch->addFile(logFile.url().path());
	/*
	d->fileWatch.setInterval(1000);
	connect(& (d->fileWatch), SIGNAL(timeout()), this, SLOT(watchFile()));
	*/

	logDebug() << "Starting " << logFile.url().path() << endl;


}


KioLogFileReader::~KioLogFileReader() {
	delete d;
}

void KioLogFileReader::open() {
	logDebug() << "Opening..." << endl;
	d->fileJob = KIO::open(d->logFile.url(), QIODevice::ReadOnly | QIODevice::Text);

	connect(d->fileJob, SIGNAL(open(KIO::Job*)), this, SLOT(openDone(KIO::Job*)));
	connect(d->fileJob, SIGNAL(close(KIO::Job*)), this, SLOT(closeDone(KIO::Job*)));

	connect(d->fileJob, SIGNAL(data(KIO::Job*, const QByteArray&)), this, SLOT(dataReceived(KIO::Job*, const QByteArray&)));
	connect(d->fileJob, SIGNAL(mimetype(KIO::Job*, const QString&)), this, SLOT(mimetypeReceived(KIO::Job*, const QString&)));

	logDebug() << "File opened." << endl;
}

void KioLogFileReader::close() {
	d->fileJob->close();
}

void KioLogFileReader::openDone(KIO::Job* job) {
	logDebug() << "Opening done..." << endl;

	d->fileJob->read(READ_SIZE);

}
void KioLogFileReader::closeDone(KIO::Job* job) {
	logDebug() << "Closing done..." << endl;
}

void KioLogFileReader::dataReceived(KIO::Job* job, const QByteArray& data) {
	if (job != d->fileJob) {
		logDebug() << "Not the good job" << endl;
		return;
	}

	if (data.isEmpty()) {
		return;
	}

	//logDebug() << "Receiving data... (" << d->totalRead << ")" << endl;
	d->buffer.append(QLatin1String( data ));
	d->totalRead += data.size();

	emitCompleteLines();

	logDebug() << "Total read : " << d->totalRead << " of " << d->fileJob->size() << endl;
	if (d->totalRead < d->fileJob->size()) {
		d->fileJob->read(READ_SIZE);
	}
	else {
		logDebug() << "Entire file read, beginning file watching..." << endl;
		d->fileWatch->startScan();

	}


	//logDebug() << "Data received : " << d->buffer << endl;

	//d->totalRead++;
}

void KioLogFileReader::emitCompleteLines() {

	int endLinePos = d->buffer.indexOf(QLatin1String( "\n" ));
	forever {
		if (endLinePos==-1)
			break;

		emit lineRead(d->buffer.left(endLinePos));

		//Remove the emitted line and the end line character
		d->buffer.remove(0, endLinePos+1);

		endLinePos = d->buffer.indexOf(QLatin1String( "\n" ));
	}

	//If this is the end line and it does not terminate by a \n, we return it
	if (d->totalRead == d->fileJob->size()) {
		emit lineRead(d->buffer);
		d->buffer.clear();

	}

}

void KioLogFileReader::mimetypeReceived(KIO::Job* job, const QString& type) {
	logDebug() << "Mimetype received " << type << endl;
}

void KioLogFileReader::watchFile(const QString& path) {
	logDebug() << "Watch file : size : " << path << endl;
}

#include "kioLogFileReader.moc"
