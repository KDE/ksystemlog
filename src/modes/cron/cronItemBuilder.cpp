/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cronItemBuilder.h"

QString CronItemBuilder::createFormattedText(LogLine *line) const
{
    QString result;

    result.append(QLatin1String("<table>"));

    QListIterator<QString> it(line->logItems());

    result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
    result.append(labelMessageFormat(i18n("Hostname:"), it.next()));
    result.append(labelMessageFormat(i18n("Process:"), it.next()));
    result.append(labelMessageFormat(i18n("User:"), it.next()));
    result.append(labelMessageFormat(i18n("Level:"), line->logLevel()->name()));
    result.append(labelMessageFormat(i18n("Original file:"), line->sourceFileName()));

    result.append(QLatin1String("</table>"));

    return result;
}
