#pragma once

#include <QMap>

#include "auditConvertor.h"
#include "fileAnalyzer.h"

#include "auditLogMode.h"

class AuditAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit AuditAnalyzer(LogMode *logMode);

    LogViewColumns initColumns() override;

    void setLogFiles(const QVector<LogFile> &logFiles) override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;
    Analyzer::LogFileSortMode logFileSortMode() override;
    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile) override;

private:
    void deleteLogFiles();

    /**
     * Parse and insert the buffered lines in the model
     * Returns the count of inserted lines
     */
    int insertLines(const QStringList &bufferedLines, const LogFile &logFile, ReadingMode readingMode);

    /**
     * Parse and insert a line in the model
     * Returns false if it was not inserted, true if it was
     */
    bool insertLine(const QStringList &event, const LogFile &originalFile, ReadingMode readingMode);

    QString getMsgField(const QString &logLine);

    LogLine *parseEvent(const QStringList &event, const LogFile &originalFile);

private Q_SLOTS:
    void logFileChanged(LogFileReader *logFileReader, Analyzer::ReadingMode readingMode, const QStringList &content);

private:
    AuditConvertor m_conv;
};
