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

#ifndef CUPS_PAGE_ITEM_BUILDER_H
#define CUPS_PAGE_ITEM_BUILDER_H

#include <QString>

#include "logModeItemBuilder.h"

#include <KLocalizedString>

#include "logging.h"

#include "logLine.h"
#include "logViewWidgetItem.h"
#include "logMode.h"

class CupsPageItemBuilder : public LogModeItemBuilder
{
public:
    CupsPageItemBuilder() {}

    virtual ~CupsPageItemBuilder() {}

    QString createFormattedText(LogLine *line) const override
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
};

#endif // _CUPS_PAGE_ITEM_BUILDER_H
