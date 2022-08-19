/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>
    SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStringList>

namespace JournaldTypes
{
struct JournalAddress {
    QString address;
    quint16 port = 0;
    bool https = false;
};

bool operator<(const JournalAddress &a1, const JournalAddress &a2);

struct JournalFilters {
    QStringList syslogIdentifiers;
    QStringList systemdUnits;
};

enum class JournaldAnalyzerType { Local, Network };

struct JournaldAnalyzerOptions {
    JournaldAnalyzerType analyzerType = JournaldAnalyzerType::Local;
    QString filter;
    JournalAddress address;
};

extern const char *ConsoleColorEscapeSequence;
}
