/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *   Copyright (C) 2015 by Vyacheslav Matyushin                            *
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

#ifndef JOURNALD_ANALYZER_H
#define JOURNALD_ANALYZER_H

#include "analyzer.h"
#include "logFile.h"

class JournaldAnalyzer : public Analyzer
{
    Q_OBJECT

public:
    explicit JournaldAnalyzer(LogMode *mode);

    LogViewColumns initColumns() override;

    void setLogFiles(const QList<LogFile> &logFiles) override;

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

#endif // _JOURNALD_ANALYZER_H
