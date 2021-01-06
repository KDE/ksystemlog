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

#ifndef JOURNALD_LOG_MODE_H
#define JOURNALD_LOG_MODE_H

/**
 * Journald Log Mode Identifier
 */
#define JOURNALD_LOG_MODE_ID "journaldLogMode"

/**
 * Journald Log Icon
 */
#define JOURNALD_MODE_ICON "preferences-system-startup"

#include <QList>

#include "logFile.h"
#include "logMode.h"

#include "journaldTypes.h"

using namespace JournaldTypes;

Q_DECLARE_METATYPE(JournaldAnalyzerOptions)

class JournaldLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit JournaldLogMode();

    Analyzer *createAnalyzer(const QVariant &analyzerOptions = QVariant()) override;

    QList<LogFile> createLogFiles() override;

    void updateJournalFilters(const JournalAddress &address, const JournalFilters &filters);
    JournalFilters filters(const JournalAddress &address) const;

private:
    QMap<JournalAddress, JournalFilters> mRemoteJournalFilters;
};

#endif // _JOURNALD_LOG_MODE_H
