/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>
    SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "logModeConfiguration.h"

#include "defaults.h"

#include "journaldLogMode.h"
#include "journaldTypes.h"

#include "ksystemlogConfig.h"

using namespace JournaldTypes;

class JournaldConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    enum EntriesType {
        EntriesAll = 0,
        EntriesCurrentUser = 1,
        EntriesSystem = 2
    };

    JournaldConfiguration();

    bool displayCurrentBootOnly() const;
    void setDisplayCurrentBootOnly(bool displayCurrentBootOnly);

    EntriesType entriesType();
    void setEntriesType(EntriesType entriesType);

    QVector<JournalAddress> remoteJournals() const;
    void setRemoteJournals(const QVector<JournalAddress> &remoteJournals);

private:
    bool mDisplayCurrentBootOnly;
    int mEntriesType;
    QStringList mRemoteJournals;
};
