/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cupsPageItemBuilder.h"

QString CupsPageItemBuilder::createFormattedText(LogLine *line) const
{
    QString result;

    QListIterator<QString> it(line->logItems());

    result.append(QLatin1String("<table>"));

    result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
    result.append(labelMessageFormat(i18n("Level:"), line->logLevel()->name()));
    result.append(labelMessageFormat(i18n("Printer:"), it.next()));
    result.append(labelMessageFormat(i18n("Username:"), it.next()));
    result.append(labelMessageFormat(i18n("Job Id:"), it.next()));
    result.append(labelMessageFormat(i18n("Page Number:"), it.next()));
    result.append(labelMessageFormat(i18n("Num Copies:"), it.next()));
    result.append(labelMessageFormat(i18n("Job Billing:"), it.next()));

    result.append(QLatin1String("</table>"));

    return result;
}
