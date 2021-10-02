/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include <QByteArray>
#include <QObject>
#include <QString>

#include "logFile.h"

namespace KIO
{
class Job;
class FileJob;
}
class KDirWatch;
/**
 * TODO Inherits from LogFileReader
 */
class KioLogFileReader : public QObject
{
    Q_OBJECT

public:
    explicit KioLogFileReader(const LogFile &logFile);

    ~KioLogFileReader() override;

    void open();
    void close();

Q_SIGNALS:
    void lineRead(const QString &);

private Q_SLOTS:
    void openDone(KIO::Job *job);
    void closeDone(KIO::Job *job);
    void dataReceived(KIO::Job *job, const QByteArray &data);
    void mimetypeReceived(KIO::Job *job, const QString &type);

    void watchFile(const QString &path);

private:
    void emitCompleteLines();
    const LogFile mLogFile;

    KIO::FileJob *mFileJob = nullptr;

    QString mBuffer;
    qulonglong mTotalRead = 0;

    KDirWatch *const mFileWatch;
};

