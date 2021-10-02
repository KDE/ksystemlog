/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *   SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "journaldTypes.h"

namespace JournaldTypes
{
bool operator<(const JournalAddress &a1, const JournalAddress &a2)
{
    return (a1.address < a2.address) && (a1.port < a2.port);
}

const char *ConsoleColorEscapeSequence = "\x1B.+m{1}";
}
