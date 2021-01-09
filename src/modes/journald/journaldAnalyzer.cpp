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

void JournaldAnalyzer::setLogFiles(const QList<LogFile> &logFiles)
{
    Q_UNUSED(logFiles)
    // Do nothing.
}

int JournaldAnalyzer::updateModel(QList<JournalEntry> &entries, ReadingMode readingMode)
{
    const int entriesNum = entries.size();
    for (int i = 0; i < entriesNum; i++) {
        const JournalEntry &entry = entries.at(i);
        const QStringList itemComponents {entry.unit, entry.message};
        auto *line = new LogLine(mLogLineInternalIdGenerator++, entry.date, itemComponents, QString(),
                                    Globals::instance().logLevelByPriority(entry.priority), mLogMode);
        line->setRecent(readingMode == UpdatingRead);
        mLogViewModel->insertNewLogLine(line);

        if (readingMode == FullRead) {
            informOpeningProgress(i, entriesNum);
        }
    }
    return entriesNum;
}
