/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "apacheAccessItemBuilder.h"

QString ApacheAccessItemBuilder::createFormattedText(LogLine *line) const
{
    QString result;

    QListIterator<QString> it(line->logItems());

    result.append(QLatin1String("<table>"));

    result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
    result.append(labelMessageFormat(i18n("Level:"), line->logLevel()->name()));
    result.append(labelMessageFormat(i18n("Host Name:"), it.next()));
    result.append(labelMessageFormat(i18n("Identification:"), it.next()));
    result.append(labelMessageFormat(i18n("Username:"), it.next()));
    result.append(labelMessageFormat(i18n("HTTP Response:"), it.next()));
    result.append(labelMessageFormat(i18n("Bytes Sent:"), it.next()));
    result.append(labelMessageFormat(i18n("Agent Identity:"), it.next()));
    result.append(labelMessageFormat(i18n("HTTP Request:"), it.next()));

    result.append(QLatin1String("</table>"));

    return result;
}
