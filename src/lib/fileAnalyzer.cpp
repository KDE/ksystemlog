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

#include "fileAnalyzer.h"

#include <KLocalizedString>
#include <QElapsedTimer>

#include "ksystemlogConfig.h"
#include "logging.h"

#include "logViewModel.h"

#include "logFileReader.h"
#include "logMode.h"

FileAnalyzer::FileAnalyzer(LogMode *logMode)
    : Analyzer(logMode)
{
}

FileAnalyzer::~FileAnalyzer()
{
    deleteLogFiles();

    // logMode is managed by Globals
    // logViewModel is managed by LogViewWidget
}

void FileAnalyzer::watchLogFiles(bool enabled)
{
    // Enable the log file watching, by revert order to read the most top file at last, and be sure its line
    // will be kept

    QListIterator<LogFileReader *> it(mLogFileReaders);
    it.toBack();
    while (it.hasPrevious()) {
        LogFileReader *logFileReader = it.previous();
        logFileReader->watchFile(enabled);
    }
}

void FileAnalyzer::deleteLogFiles()
{
    watchLogFiles(false);

    // Remove the watching on the monitored files
    foreach (LogFileReader *logFileReader, mLogFileReaders) {
        logDebug() << "Remove file : " << logFileReader->logFile().url().toLocalFile();
        delete logFileReader;
    }

    mLogFileReaders.clear();
}

void FileAnalyzer::setLogFiles(const QList<LogFile> &logFiles)
{
    // Remove previous files
    deleteLogFiles();

    for (const LogFile &logFile : logFiles) {
        LogFileReader *logFileReader = createLogFileReader(logFile);
        mLogFileReaders.append(logFileReader);

        connect(logFileReader, &LogFileReader::contentChanged, this, &FileAnalyzer::logFileChanged);
        connect(logFileReader, &LogFileReader::statusBarChanged, this, &Analyzer::statusBarChanged);
        connect(logFileReader, &LogFileReader::errorOccured, this, &Analyzer::errorOccured);
    }
}

void FileAnalyzer::logFileChanged(LogFileReader *logFileReader, ReadingMode readingMode, const QStringList &content)
{
    const QString filePath = logFileReader->logFile().url().toLocalFile();
    if (readingMode == Analyzer::FullRead) {
        logDebug() << "File " << filePath << " has been modified on full read.";
    } else {
        logDebug() << "File " << filePath << " has been modified on partial read";
    }

    if (mParsingPaused) {
        logDebug() << "Pause enabled. Nothing read.";
        return;
    }

    logDebug() << "Locking file modifications of " << filePath;
    mInsertionLocking.lock();
    logDebug() << "Unlocking file modifications of " << filePath;

    QElapsedTimer benchmark;
    benchmark.start();

    int insertedLogLineCount;

    mLogViewModel->startingMultipleInsertions();

    if (readingMode == Analyzer::UpdatingRead) {
        insertedLogLineCount = insertLines(content, logFileReader->logFile(), Analyzer::UpdatingRead);
    } else {
        logDebug() << "Reading file " << filePath;

        Q_EMIT statusBarChanged(i18n("Opening '%1'...", filePath));

        // Inform that we are now reading the "index" file
        Q_EMIT readFileStarted(*mLogMode, logFileReader->logFile(), mLogFileReaders.count() - mLogFileReaders.indexOf(logFileReader), mLogFileReaders.count());

        insertedLogLineCount = insertLines(content, logFileReader->logFile(), Analyzer::FullRead);

        Q_EMIT statusBarChanged(i18n("Log file '%1' loaded successfully.", filePath));
    }

    mLogViewModel->endingMultipleInsertions(readingMode, insertedLogLineCount);

    // Inform connected LoadingBar that the reading is now finished
    Q_EMIT readEnded();

    // Inform LogManager that new lines have been added
    Q_EMIT logUpdated(insertedLogLineCount);

    // Inform MainWindow status bar
    Q_EMIT statusBarChanged(i18n("Log file '%1' has changed.", filePath));

    logDebug() << "Updating log files in " << benchmark.elapsed() << " ms";

    mInsertionLocking.unlock();
}

int FileAnalyzer::insertLines(const QStringList &bufferedLines, const LogFile &logFile, ReadingMode readingMode)
{
    logDebug() << "Inserting lines...";

    // If there is no line
    if (bufferedLines.isEmpty()) {
        logWarning() << "File is empty : " << logFile.url().toLocalFile();
    }

    int stop = 0;
    int currentPosition = 0;

    QListIterator<QString> it(bufferedLines);
    /**
     * If the log file is sorted, then we can ignore the first lines
     * if there are more lines in the log file than the max lines
     *
     * TODO Read revertly the file and ignore last lines if we are in Descending mode
     */
    logDebug() << "Log file Sort mode is " << logFileSortMode();
    if (logFileSortMode() == Analyzer::AscendingSortedLogFile) {
        // Calculate how many lines we will ignore
        if (bufferedLines.size() > KSystemLogConfig::maxLines()) {
            stop = bufferedLines.size() - KSystemLogConfig::maxLines();
        }

        // Ignore those lines
        while (currentPosition < stop) {
            it.next();
            ++currentPosition;
        }
    }

    int insertedLogLineCount = 0;
    while (currentPosition < bufferedLines.size()) {
        const QString buffer(it.next());

        const bool inserted = insertLine(buffer, logFile, readingMode);
        if (inserted) {
            insertedLogLineCount++;
        }

        if (readingMode == Analyzer::FullRead) {
            informOpeningProgress(currentPosition, (bufferedLines.size() - 1) - stop);
        }

        ++currentPosition;
    }

    logDebug() << "Total read lines :" << (bufferedLines.size() - stop) << "(" << logFile.url().toLocalFile() << ")";

    return insertedLogLineCount;
}

bool FileAnalyzer::insertLine(const QString &buffer, const LogFile &originalFile, ReadingMode readingMode)
{
    LogLine *line = parseMessage(buffer, originalFile);

    // Invalid log line
    if (!line) {
        return false;
    }

    // On full reading, it is not needed to display the recent status
    if (readingMode == Analyzer::FullRead) {
        line->setRecent(false);
    }

    return mLogViewModel->insertNewLogLine(line);
}
