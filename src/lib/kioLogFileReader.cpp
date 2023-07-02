/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kioLogFileReader.h"

#include <QIODevice>

#include <kio/filejob.h>
#include <kio/job.h>

#include <KDirWatch>

#include "ksystemlog_debug.h"

#define READ_SIZE 10

KioLogFileReader::KioLogFileReader(const LogFile &logFile)
    : mLogFile(logFile)
    , mFileWatch(new KDirWatch(this))
{
    connect(mFileWatch, &KDirWatch::dirty, this, &KioLogFileReader::watchFile);
    mFileWatch->addFile(logFile.url().toLocalFile());
    /*
    fileWatch.setInterval(1000);
    connect(& (fileWatch), SIGNAL(timeout()), this, SLOT(watchFile()));
    */

    qCDebug(KSYSTEMLOG) << "Starting " << logFile.url().toLocalFile();
}

KioLogFileReader::~KioLogFileReader()
{
}

void KioLogFileReader::open()
{
    qCDebug(KSYSTEMLOG) << "Opening...";
    mFileJob = KIO::open(mLogFile.url(), QIODevice::ReadOnly | QIODevice::Text);

    connect(mFileJob, &KIO::FileJob::open, this, &KioLogFileReader::openDone);
    connect(mFileJob, SIGNAL(close(KIO::Job *)), this, SLOT(closeDone(KIO::Job *)));

    connect(mFileJob, &KIO::FileJob::data, this, &KioLogFileReader::dataReceived);
    connect(mFileJob, &KIO::FileJob::mimeTypeFound, this, &KioLogFileReader::mimetypeReceived);
    qCDebug(KSYSTEMLOG) << "File opened.";
}

void KioLogFileReader::close()
{
    mFileJob->close();
}

void KioLogFileReader::openDone(KIO::Job * /*job*/)
{
    qCDebug(KSYSTEMLOG) << "Opening done...";

    mFileJob->read(READ_SIZE);
}

void KioLogFileReader::closeDone(KIO::Job * /*job*/)
{
    qCDebug(KSYSTEMLOG) << "Closing done...";
}

void KioLogFileReader::dataReceived(KIO::Job *job, const QByteArray &data)
{
    if (job != mFileJob) {
        qCDebug(KSYSTEMLOG) << "Not the good job";
        return;
    }

    if (data.isEmpty()) {
        return;
    }

    // qCDebug(KSYSTEMLOG) << "Receiving data... (" << totalRead << ")";
    mBuffer.append(QLatin1String(data));
    mTotalRead += data.size();

    emitCompleteLines();

    qCDebug(KSYSTEMLOG) << "Total read : " << mTotalRead << " of " << mFileJob->size();
    if (mTotalRead < mFileJob->size()) {
        mFileJob->read(READ_SIZE);
    } else {
        qCDebug(KSYSTEMLOG) << "Entire file read, beginning file watching...";
        mFileWatch->startScan();
    }

    // qCDebug(KSYSTEMLOG) << "Data received : " << buffer;

    // totalRead++;
}

void KioLogFileReader::emitCompleteLines()
{
    int endLinePos = mBuffer.indexOf(QLatin1String("\n"));
    while (true) {
        if (endLinePos == -1) {
            break;
        }

        Q_EMIT lineRead(mBuffer.left(endLinePos));

        // Remove the emitted line and the end line character
        mBuffer.remove(0, endLinePos + 1);

        endLinePos = mBuffer.indexOf(QLatin1String("\n"));
    }

    // If this is the end line and it does not terminate by a \n, we return it
    if (mTotalRead == mFileJob->size()) {
        Q_EMIT lineRead(mBuffer);
        mBuffer.clear();
    }
}

void KioLogFileReader::mimetypeReceived(KIO::Job * /*job*/, const QString &type)
{
    qCDebug(KSYSTEMLOG) << "Mimetype received " << type;
}

void KioLogFileReader::watchFile(const QString &path)
{
    qCDebug(KSYSTEMLOG) << "Watch file : size : " << path;
}

#include "moc_kioLogFileReader.cpp"
