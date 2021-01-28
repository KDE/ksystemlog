#include <QElapsedTimer>
#include <QRegularExpression>

#include <KLocalizedString>

#include "ksystemlogConfig.h"
#include "logging.h"

#include "logViewModel.h"

#include "auditAnalyzer.h"
#include "localLogFileReader.h"

AuditAnalyzer::AuditAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
}

LogViewColumns AuditAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

void AuditAnalyzer::setLogFiles(const QVector<LogFile> &logFiles)
{
    // Remove previous files
    deleteLogFiles();

    for (const LogFile &logFile : logFiles) {
        LogFileReader *logFileReader = createLogFileReader(logFile);
        mLogFileReaders.append(logFileReader);

        connect(logFileReader, &LogFileReader::contentChanged, this, &AuditAnalyzer::logFileChanged);
        connect(logFileReader, &LogFileReader::statusBarChanged, this, &Analyzer::statusBarChanged);
        connect(logFileReader, &LogFileReader::errorOccured, this, &Analyzer::errorOccured);
    }
}

LogFileReader *AuditAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode AuditAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

LogLine *AuditAnalyzer::parseMessage(const QString &logLine, const LogFile &originalFile)
{
    if (logLine.length() < 15) {
        return nullptr;
    }

    static QRegularExpression re(QStringLiteral("^type=(.*)\\s.*msg=audit\\((\\d*\\.\\d*).*"));
    QRegularExpressionMatch match = re.match(logLine);

    if (!match.hasMatch()) {
        return nullptr;
    }

    const QString messageType = match.captured(1);

    const qint64 msecs = qint64(match.captured(2).toDouble() * 1000.);

    const QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(msecs);

    QString message(logLine);
    re.setPattern(QStringLiteral("^type=.*\\):\\s"));
    message.remove(re);

    QStringList list;
    list.append(messageType);
    list.append(message);

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, list, originalFile.url().path(), originalFile.defaultLogLevel(), mLogMode);
}

void AuditAnalyzer::deleteLogFiles()
{
    watchLogFiles(false);

    // Remove the watching on the monitored files
    foreach (LogFileReader *logFileReader, mLogFileReaders) {
        logDebug() << "Remove file : " << logFileReader->logFile().url().path();
        delete logFileReader;
    }

    mLogFileReaders.clear();
}

int AuditAnalyzer::insertLines(const QStringList &bufferedLines, const LogFile &logFile, ReadingMode readingMode)
{
    logDebug() << "Inserting lines...";

    // If there is no line
    if (bufferedLines.isEmpty()) {
        logWarning() << "File is empty : " << logFile.url().path();
    }

    int stop = 0;
    int currentPosition = 0;

    QList<QStringList> eventLines;
    QStringList event;
    QString msgField;
    QString curMsgField;

    for (const auto &line : bufferedLines) {
        msgField = getMsgField(line);

        if (msgField != curMsgField) {
            eventLines.append(event);
            curMsgField = msgField;
            event.clear();
        }

        event.push_front(line);
    }

    eventLines.append(event);

    QListIterator<QStringList> it(eventLines);
    /**
     * If the log file is sorted, then we can ignore the first lines
     * if there are more lines in the log file than the max lines
     *
     * TODO Read revertly the file and ignore last lines if we are in Descending mode
     */
    logDebug() << "Log file Sort mode is " << logFileSortMode();
    if (logFileSortMode() == Analyzer::AscendingSortedLogFile) {
        // Calculate how many lines we will ignore
        if (eventLines.size() > KSystemLogConfig::maxLines()) {
            stop = eventLines.size() - KSystemLogConfig::maxLines();
        }

        // Ignore those lines
        while (currentPosition < stop) {
            it.next();
            ++currentPosition;
        }
    }

    int insertedLogLineCount = 0;

    while (currentPosition < eventLines.size()) {
        if (insertLine(it.next(), logFile, readingMode)) {
            insertedLogLineCount++;
        }

        if (readingMode == Analyzer::FullRead) {
            informOpeningProgress(currentPosition, (eventLines.size() - 1) - stop);
        }

        ++currentPosition;
    }

    return insertedLogLineCount;
}

bool AuditAnalyzer::insertLine(const QStringList &event, const LogFile &originalFile, ReadingMode readingMode)
{
    LogLine *line = parseEvent(event, originalFile);

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

void AuditAnalyzer::logFileChanged(LogFileReader *logFileReader, ReadingMode readingMode, const QStringList &content)
{
    const QString filePath = logFileReader->logFile().url().path();
    if (readingMode == Analyzer::FullRead) {
        logDebug() << "File " << filePath << " has been modified on full read.";
    } else {
        logDebug() << "File " << filePath << " has been modified on partial read";
    }

    if (mParsingPaused == true) {
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

QString AuditAnalyzer::getMsgField(const QString &logLine)
{
    static const QRegularExpression re(QStringLiteral("^.*msg=audit\\((\\d*\\.\\d*:\\d*)\\)"));
    const QRegularExpressionMatch match = re.match(logLine);

    if (!match.hasMatch()) {
        return QString();
    } else {
        return match.captured(1);
    }
}

LogLine *AuditAnalyzer::parseEvent(const QStringList &event, const LogFile &originalFile)
{
    if (event.isEmpty()) {
        return nullptr;
    }

    QRegularExpression re(QStringLiteral("^.*msg=audit\\((\\d*\\.\\d*).*"));
    QRegularExpressionMatch match = re.match(event.at(0));

    if (!match.hasMatch()) {
        return nullptr;
    }

    const qint64 msecs = qint64(match.captured(1).toDouble() * 1000.);
    const QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(msecs);

    QStringList messages;
    QString message;

    for (const auto &msg : event) {
        message = m_conv.convertMessage(msg);
        re.setPattern(QStringLiteral("^type="));
        message.remove(re);
        re.setPattern(QStringLiteral("\\smsg=audit\\(\\d*\\.\\d*:\\d*\\)"));
        message.remove(re);
        messages.append(message);
    }

    QStringList list;
    list.append(messages.join(QStringLiteral(" \n")));

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, list, originalFile.url().path(), originalFile.defaultLogLevel(), mLogMode);
}
