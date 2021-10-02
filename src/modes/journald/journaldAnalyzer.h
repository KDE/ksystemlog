/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>
    SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "analyzer.h"
#include "logFile.h"

class JournaldAnalyzer : public Analyzer
{
    Q_OBJECT

public:
    explicit JournaldAnalyzer(LogMode *mode);

    LogViewColumns initColumns() override;

    void setLogFiles(const QVector<LogFile> &logFiles) override;

    virtual QStringList units() const = 0;

    virtual QStringList syslogIdentifiers() const = 0;

protected:
    struct JournalEntry {
        QDateTime date;
        QString unit;
        QString message;
        int priority;
        QString bootID;
    };

    int updateModel(QList<JournalEntry> &entries, ReadingMode readingMode);
};

