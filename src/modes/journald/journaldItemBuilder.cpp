/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "journaldItemBuilder.h"

#include "logLine.h"

#include <KLocalizedString>

#include <QString>

QString JournaldItemBuilder::createFormattedText(LogLine *line) const
{
    QString result;

    QListIterator<QString> it(line->logItems());

    result.append(QLatin1String("<table>"));
    result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
    result.append(labelMessageFormat(i18n("Priority:"), line->logLevel()->name()));
    result.append(labelMessageFormat(i18n("Unit:"), it.next()));
    result.append(QLatin1String("</table>"));

    return result;
}

QString JournaldItemBuilder::createToolTipText(LogLine *line) const
{
    return createFormattedText(line);
}
