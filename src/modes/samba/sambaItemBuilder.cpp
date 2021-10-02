/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sambaItemBuilder.h"

QString SambaItemBuilder::createFormattedText(LogLine *line) const
{
    QString result;

    QListIterator<QString> it(line->logItems());

    result.append(QLatin1String("<table>"));

    result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
    result.append(labelMessageFormat(i18n("Level:"), line->logLevel()->name()));
    result.append(labelMessageFormat(i18n("Source File:"), it.next()));
    result.append(labelMessageFormat(i18n("Function:"), it.next()));
    result.append(labelMessageFormat(i18n("Line:"), it.next()));

    result.append(QLatin1String("</table>"));

    return result;
}
