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

#ifndef _LOG_MODE_ITEM_BUILDER_H_
#define _LOG_MODE_ITEM_BUILDER_H_

#include <QString>
#include <QDateTime>

class LogViewWidgetItem;
class LogLine;

class LogModeItemBuilder
{
public:
    LogModeItemBuilder();

    virtual ~LogModeItemBuilder();

    virtual void prepareItem(LogViewWidgetItem *item) const;

    virtual QString createFormattedText(LogLine *line) const;
    virtual QString createToolTipText(LogLine *line) const;

protected:
    QString labelMessageFormat(const QString &label, const QString &value) const;
    QString messageFormat(const QString &message) const;

    virtual QString formatDate(const QDateTime &dateTime) const;
};

#endif // _LOG_MODE_ITEM_BUILDER_H_
