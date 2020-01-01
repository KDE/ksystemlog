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

#ifndef X_SESSION_ITEM_BUILDER_H
#define X_SESSION_ITEM_BUILDER_H

#include <QString>

#include <KLocalizedString>

#include "logging.h"

#include "logLine.h"
#include "logViewWidgetItem.h"
#include "logMode.h"

#include "logModeItemBuilder.h"

class LogLine;

class XSessionItemBuilder : public LogModeItemBuilder
{
public:
    XSessionItemBuilder() {}

    ~XSessionItemBuilder() override {}

    void prepareItem(LogViewWidgetItem *item) const override
    {
        LogLine *line = item->logLine();

        item->setText(0, QLatin1String(""));

        int i = 1;
        foreach (const QString &label, line->logItems()) {
            item->setText(i, label);
            i++;
        }

        item->setIcon(0, line->logLevel()->icon());
    }

    QString createToolTipText(LogLine *line) const override
    {
        QString result;

        QListIterator<QString> it(line->logItems());

        result.append(QLatin1String("<table>"));

        QString type = it.next();
        if (type.isEmpty())
            result.append(labelMessageFormat(i18n("Program:"), i18n("none")));
        else
            result.append(labelMessageFormat(i18n("Program:"), type));

        result.append(labelMessageFormat(i18n("Original file:"), line->sourceFileName()));

        result.append(QLatin1String("</table>"));

        return result;
    }

    QString createFormattedText(LogLine *line) const override
    {
        // It uses the same formating than the tool tip
        return createToolTipText(line);
    }
};

#endif // _X_SESSION_ITEM_BUILDER_H
