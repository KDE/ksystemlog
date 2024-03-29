/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "apacheAnalyzer.h"

ApacheAnalyzer::ApacheAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
    initializeTypeLevels();
}

LogViewColumns ApacheAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Client"), true, false));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

LogFileReader *ApacheAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode ApacheAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

LogLine *ApacheAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    QString line(logLine);

    QDate date;
    QTime time;

    QString level;

    // Temporary variable
    int squareBracket;

    // Special case which sometimes happens
    if (line.indexOf(QLatin1String("[client")) == 0) {
        date = QDate::currentDate();
        time = QTime::currentTime();
        level = QStringLiteral("notice");
    } else {
        // The Date
        int const dateBegin = line.indexOf(QLatin1String("["));
        int const dateEnd = line.indexOf(QLatin1String("]"));

        const QString strDate = line.mid(dateBegin + 1, dateEnd - dateBegin - 1);

        const QString month = strDate.mid(4, 3);

        const QString day = strDate.mid(8, 2);

        const QString hour = strDate.mid(11, 2);
        const QString min = strDate.mid(14, 2);
        const QString sec = strDate.mid(17, 2);

        const QString year = strDate.mid(20, 4);

        date = QDate(year.toInt(), ParsingHelper::instance()->parseSyslogMonth(month), day.toInt());
        time = QTime(hour.toInt(), min.toInt(), sec.toInt());

        line.remove(0, dateEnd + 3);

        // The log level
        squareBracket = line.indexOf(QLatin1String("]"));
        level = line.left(squareBracket);
        line.remove(0, squareBracket + 2);
    }

    // The client
    int const beginSquareBracket = line.indexOf(QLatin1String("[client"));
    squareBracket = line.indexOf(QLatin1String("]"));
    QString client;
    if (beginSquareBracket == -1 || squareBracket == -1) {
        client = QLatin1String("");
    } else {
        client = line.mid(8, squareBracket - 8); // 8=strlen("[client ")
        line.remove(0, squareBracket + 2);
    }

    QStringList list;
    list.append(client);
    list.append(line);

    return new LogLine(mLogLineInternalIdGenerator++, QDateTime(date, time), list, originalLogFile.url().toLocalFile(), findLogLevel(level), mLogMode);
}

void ApacheAnalyzer::initializeTypeLevels()
{
    mMapTypeLevels[QStringLiteral("notice")] = Globals::instance().informationLogLevel();
    mMapTypeLevels[QStringLiteral("warn")] = Globals::instance().warningLogLevel();
    mMapTypeLevels[QStringLiteral("error")] = Globals::instance().errorLogLevel();
}

LogLevel *ApacheAnalyzer::findLogLevel(const QString &type)
{
    QMap<QString, LogLevel *>::iterator it;

    it = mMapTypeLevels.find(type);
    if (it != mMapTypeLevels.end()) {
        return *it;
    } else {
        qCCritical(KSYSTEMLOG) << "New Log Level detected: Please send this log file to the KSystemLog developer to add it (" << type << ")";
        return Globals::instance().noLogLevel();
    }
}

#include "moc_apacheAnalyzer.cpp"
