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

#ifndef JOURNALD_TYPES_H
#define JOURNALD_TYPES_H

#include <QStringList>

namespace JournaldTypes {
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

enum class JournaldAnalyzerType {
    Local, Network
};

struct JournaldAnalyzerOptions {
    JournaldAnalyzerType analyzerType = JournaldAnalyzerType::Local;
    QString filter;
    JournalAddress address;
};

extern const char *ConsoleColorEscapeSequence;
}

#endif // _JOURNALD_TYPES_H
