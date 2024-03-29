/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "fileAnalyzer.h"

#include <KLocalizedString>
#include <QElapsedTimer>

#include "ksystemlogConfig.h"
#include "ksystemlog_debug.h"

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
    qDeleteAll(mLogFileReaders);
    mLogFileReaders.clear();
}

void FileAnalyzer::setLogFiles(const QVector<LogFile> &logFiles)
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
        qCDebug(KSYSTEMLOG) << "File " << filePath << " has been modified on full read.";
    } else {
        qCDebug(KSYSTEMLOG) << "File " << filePath << " has been modified on partial read";
    }

    if (mParsingPaused) {
        qCDebug(KSYSTEMLOG) << "Pause enabled. Nothing read.";
        return;
    }

    qCDebug(KSYSTEMLOG) << "Locking file modifications of " << filePath;
    mInsertionLocking.lock();
    qCDebug(KSYSTEMLOG) << "Unlocking file modifications of " << filePath;

    QElapsedTimer benchmark;
    benchmark.start();

    int insertedLogLineCount;

    mLogViewModel->startingMultipleInsertions();

    if (readingMode == Analyzer::UpdatingRead) {
        insertedLogLineCount = insertLines(content, logFileReader->logFile(), Analyzer::UpdatingRead);
    } else {
        qCDebug(KSYSTEMLOG) << "Reading file " << filePath;

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

    qCDebug(KSYSTEMLOG) << "Updating log files in " << benchmark.elapsed() << " ms";

    mInsertionLocking.unlock();
}

int FileAnalyzer::insertLines(const QStringList &bufferedLines, const LogFile &logFile, ReadingMode readingMode)
{
    qCDebug(KSYSTEMLOG) << "Inserting lines...";

    // If there is no line
    if (bufferedLines.isEmpty()) {
        qCWarning(KSYSTEMLOG) << "File is empty : " << logFile.url().toLocalFile();
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
    qCDebug(KSYSTEMLOG) << "Log file Sort mode is " << logFileSortMode();
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

    qCDebug(KSYSTEMLOG) << "Total read lines :" << (bufferedLines.size() - stop) << "(" << logFile.url().toLocalFile() << ")";

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

#include "moc_fileAnalyzer.cpp"
