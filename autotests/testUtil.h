/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include <QList>
#include <QString>

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
     * If the model == nullptr, then a new model is created and the pointer is update
     */
    Analyzer *createAnalyzer(const QString &logModeName, LogViewModel **model) const;

    /*
     * Delete the reader provided in parameter
     */
    void destroyReader(Analyzer *analyzer) const;

    /*
     * Create a LogFile list from a single resourceFileName
     */
    QVector<LogFile> createLogFiles(const QString &resourceFileName) const;

    /*
     * Add the "addedLines" list to the file "fileName"
     */
    void addLogLines(const QString &fileName, const QStringList &addedLines) const;

    /*
     * Test every fields of the logLine with the other parameters
     */
    void testLine(LogLine *line, const QString &originalFileName, LogLevel *logLevel, const QDateTime &time, const QStringList &items) const;
};

