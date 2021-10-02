/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>
#include <QStringList>

#include "logFile.h"

#include "logFileReader.h"

class LocalLogFileReaderPrivate;

class LocalLogFileReader : public LogFileReader
{
    Q_OBJECT

public:
    explicit LocalLogFileReader(const LogFile &logFile);

    ~LocalLogFileReader() override;

    void watchFile(bool enable) override;

    void setPreviousFilePosition(long previousFilePosition);

private Q_SLOTS:
    void logFileModified();

private:
    void init();

    QIODevice *open();
    void close(QIODevice *inputDevice);

    QStringList readContent(QIODevice *inputDevice);

protected:
    LocalLogFileReader(LocalLogFileReaderPrivate &dd, const LogFile &logFile);

private:
    Q_DECLARE_PRIVATE(LocalLogFileReader)
};

