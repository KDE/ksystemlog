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
#include <QMimeDatabase>

#include <kdirwatch.h>
#include <KLocalizedString>
#include <kfilterdev.h>

#include "logFileReader.h"
#include "logFileReaderPrivate.h"

#include "logging.h"

class LocalLogFileReaderPrivate : public LogFileReaderPrivate
{
public:
    KDirWatch *watch;

    long previousFilePosition;

    /**
     * Mutex avoiding multiple logFileModified() calls
     */
    QMutex insertionLocking;
};

LocalLogFileReader::LocalLogFileReader(const LogFile &logFile)
    : LogFileReader(*new LocalLogFileReaderPrivate(), logFile)
{
    init();
}

LocalLogFileReader::LocalLogFileReader(LocalLogFileReaderPrivate &dd, const LogFile &logFile)
    : LogFileReader(dd, logFile)
{
    init();
}

LocalLogFileReader::~LocalLogFileReader()
{
    Q_D(LocalLogFileReader);

    // Delete the watching object
    delete d->watch;

    // d pointer is deleted by the parent class
}

void LocalLogFileReader::init()
{
    Q_D(LocalLogFileReader);

    d->watch = new KDirWatch();
    connect(d->watch, &KDirWatch::dirty, this, &LocalLogFileReader::logFileModified);

    // Init current file position
    d->previousFilePosition = 0;

    logDebug() << "Reading local file " << d->logFile.url().path();
}

void LocalLogFileReader::watchFile(bool enable)
{
    Q_D(LocalLogFileReader);
    QString filePath = d->logFile.url().path();

    if (enable == true) {
        logDebug() << "Monitoring file : " << filePath;

        if (d->watch->contains(filePath) == false) {
            d->watch->addFile(filePath);
        }

        // Reinit current file position
        d->previousFilePosition = 0;

        // If we enable the watching, then we first try to see if new lines have appeared
        logFileModified();
    } else {
        d->watch->removeFile(filePath);
    }
}

QIODevice *LocalLogFileReader::open()
{
    Q_D(LocalLogFileReader);
    QString filePath = d->logFile.url().path();

    if (d->logFile.url().isValid() == false) {
        QString message(i18n("This file is not valid. Please adjust it in the settings of KSystemLog."));
        emit errorOccured(i18n("File Does Not Exist"), message);
        emit statusBarChanged(message);
    }

    QMimeDatabase db;
    QString mimeType = db.mimeTypeForFile(filePath, QMimeDatabase::MatchContent).name();

    logDebug() << filePath << " : " << mimeType;
    QIODevice *inputDevice;

    // Try to see if this file exists
    QFile *file = new QFile(filePath);
    // If the file does not exist
    if (!file->exists()) {
        QString message(i18n("The file '%1' does not exist.", filePath));
        emit errorOccured(i18n("File Does Not Exist"), message);
        emit statusBarChanged(message);
        delete file;
        return NULL;
    }

    // Plain text file : we use a QFile object
    if (mimeType == QLatin1String("text/plain") || mimeType == QLatin1String("application/octet-stream")) {
        logDebug() << "Using QFile input device";

        inputDevice = file;
    }
    // Compressed file : we use the KFilterDev helper
    else {
        logDebug() << "Using KFilterDev input device";

        // inputDevice = KFilterDev::deviceForFile(filePath, mimeType);
        inputDevice = new KCompressionDevice(filePath, KFilterDev::compressionTypeForMimeType(mimeType));

        if (inputDevice == NULL) {
            QString message(i18n("Unable to uncompress the '%2' format of '%1'.", filePath, mimeType));
            emit errorOccured(i18n("Unable to Uncompress File"), message);
            emit statusBarChanged(message);
            return NULL;
        }
    }

    if (!inputDevice->open(QIODevice::ReadOnly)) {
        QString message(i18n("You do not have sufficient permissions to read '%1'.", filePath));
        emit errorOccured(i18n("Insufficient Permissions"), message);
        emit statusBarChanged(message);
        delete inputDevice;
        return NULL;
    }

    return inputDevice;
}

void LocalLogFileReader::close(QIODevice *inputDevice)
{
    inputDevice->close();
    delete inputDevice;
}

QStringList LocalLogFileReader::readContent(QIODevice *inputDevice)
{
    logDebug() << "Retrieving raw buffer...";

    Q_D(LocalLogFileReader);

    QStringList rawBuffer;

    QTextStream inputStream(inputDevice);
    while (inputStream.atEnd() == false) {
        rawBuffer.append(inputStream.readLine());
    }

    logDebug() << "Raw buffer retrieved.";

    // Get the size file for the next calculation
    d->previousFilePosition = inputDevice->size();
    logDebug() << "New file position : " << d->previousFilePosition << " (" << d->logFile.url().path() << ")"
               << endl;

    return rawBuffer;
}

void LocalLogFileReader::logFileModified()
{
    Q_D(LocalLogFileReader);

    logDebug() << "Locking log file modification...";
    if (d->insertionLocking.tryLock() == false) {
        logDebug() << "Log file modification already detected.";
        return;
    }

    QIODevice *inputDevice = open();
    if (inputDevice == NULL) {
        logCritical() << "Could not open file " << d->logFile.url().path();
        return;
    }

    // If there are new lines in the file, insert only them or this is the first time we read this file
    if (d->previousFilePosition != 0 && d->previousFilePosition <= inputDevice->size()) {
        logDebug() << "Reading from position " << d->previousFilePosition << " (" << d->logFile.url().path()
                   << ")";

        if (inputDevice->isSequential()) {
            logCritical() << "The file current position could not be modified";
        } else {
            // Place the cursor to the last line opened
            inputDevice->seek(d->previousFilePosition);
        }

        logDebug() << "Retrieving a part of the file...";

        emit contentChanged(this, Analyzer::UpdatingRead, readContent(inputDevice));

    }
    // Else reread all lines, clear log list
    else {
        logDebug() << "New file or file truncated. (Re-)Loading log file";

        emit contentChanged(this, Analyzer::FullRead, readContent(inputDevice));
    }

    close(inputDevice);

    logDebug() << "Unlocking log file modification...";
    d->insertionLocking.unlock();
}
