/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>
    SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "journaldAnalyzer.h"
#include "logViewModel.h"

#include <KLocalizedString>

JournaldAnalyzer::JournaldAnalyzer(LogMode *mode)
    : Analyzer(mode)
{
}

LogViewColumns JournaldAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, true));
    columns.addColumn(LogViewColumn(i18n("Unit"), true, true));
    columns.addColumn(LogViewColumn(i18n("Message"), true, true));
    return columns;
}

void JournaldAnalyzer::setLogFiles(const QVector<LogFile> &logFiles)
{
    Q_UNUSED(logFiles)
    // Do nothing.
}

int JournaldAnalyzer::updateModel(QList<JournalEntry> &entries, ReadingMode readingMode)
{
    const int entriesNum = entries.size();
    for (int i = 0; i < entriesNum; i++) {
        const JournalEntry &entry = entries.at(i);
        const QStringList itemComponents{entry.unit, entry.message};
        auto *line =
            new LogLine(mLogLineInternalIdGenerator++, entry.date, itemComponents, QString(), Globals::instance().logLevelByPriority(entry.priority), mLogMode);
        line->setRecent(readingMode == UpdatingRead);
        mLogViewModel->insertNewLogLine(line);

        if (readingMode == FullRead) {
            informOpeningProgress(i, entriesNum);
        }
    }
    return entriesNum;
}
