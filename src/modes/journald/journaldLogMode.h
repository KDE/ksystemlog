/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *   SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

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

    QVector<LogFile> createLogFiles() override;

    void updateJournalFilters(const JournalAddress &address, const JournalFilters &filters);
    JournalFilters filters(const JournalAddress &address) const;

private:
    QMap<JournalAddress, JournalFilters> mRemoteJournalFilters;
};

