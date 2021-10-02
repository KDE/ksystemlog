/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cupsItemBuilder.h"

QString CupsItemBuilder::createFormattedText(LogLine *line) const
{
    QString result;

    result.append(QLatin1String("<table>"));

    result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
    result.append(labelMessageFormat(i18n("Level:"), line->logLevel()->name()));

    result.append(QLatin1String("</table>"));

    return result;
}
