/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
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

    QString type = it.next();
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
