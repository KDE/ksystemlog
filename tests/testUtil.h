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

#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <QObject>

#include <QString>
#include <QList>

class LogViewModel;
class Analyzer;

#include "logFile.h"
#include "logLine.h"

#include "globals.h"

class TestUtil : public QObject
{
    Q_OBJECT

public:
    TestUtil();

    virtual ~TestUtil();

    /*
     * Init method, used to register existing Log Modes
     */
    void registerLogModeFactories() const;

    LogViewModel *defineLogViewModel(Analyzer *analyzer) const;

    /**
     * Find a reader and initialize it.
     * If the model == NULL, then a new model is created and the pointer is update
     */
    Analyzer *createAnalyzer(const QString &logModeName, LogViewModel **model) const;

    /*
     * Delete the reader provided in parameter
     */
    void destroyReader(Analyzer *analyzer) const;

    /*
     * Create a LogFile list from a single resourceFileName
     */
    QList<LogFile> createLogFiles(const QString &resourceFileName) const;

    /*
     * Add the "addedLines" list to the file "fileName"
     */
    void addLogLines(const QString &fileName, const QStringList &addedLines) const;

    /*
     * Test every fields of the logLine with the other parameters
     */
    void testLine(LogLine *line, const QString &originalFileName, LogLevel *logLevel, const QDateTime &time,
                  const QStringList &items) const;
};

#endif // TEST_UTIL_H
