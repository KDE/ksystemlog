/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "xorgItemBuilder.h"

void XorgItemBuilder::prepareItem(LogViewWidgetItem *item) const
{
    LogLine *line = item->logLine();

    item->setText(0, QLatin1String(""));

    int i = 1;
    const auto logItems = line->logItems();
    for (const QString &label : logItems) {
        item->setText(i, label);
        i++;
    }

    item->setIcon(0, line->logLevel()->icon());
}

QString XorgItemBuilder::createToolTipText(LogLine *line) const
{
    QString result;

    QListIterator<QString> it(line->logItems());

    result.append(QLatin1String("<table>"));

    const QString type = it.next();
    if (type.isEmpty()) {
        result.append(labelMessageFormat(i18n("Type:"), i18n("none")));
    } else {
        result.append(labelMessageFormat(i18n("Type:"), type));
    }

    result.append(labelMessageFormat(i18n("Original file:"), line->sourceFileName()));

    result.append(QLatin1String("</table>"));

    return result;
}

QString XorgItemBuilder::createFormattedText(LogLine *line) const
{
    // It uses the same formating than the tool tip
    return createToolTipText(line);
}
