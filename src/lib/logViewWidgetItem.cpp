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

#include "logViewWidgetItem.h"

// Qt includes


#include <QBrush>

// KDE includes
#include "logging.h"

#include "logModeItemBuilder.h"
#include "logViewWidget.h"

#include "logMode.h"

LogViewWidgetItem::LogViewWidgetItem(LogViewWidget *list, LogLine *l)
    : QTreeWidgetItem()
    , mLine(l)
{
    // Add this item to the LogLine, to let the LogLine initialize it
    mLine->setItem(this);
    list->addTopLevelItem(this);
}

LogViewWidgetItem::~LogViewWidgetItem()
{
    delete mLine;
}

LogLine *LogViewWidgetItem::logLine() const
{
    return mLine;
}

void LogViewWidgetItem::toggleToolTip(bool displayed)
{
    if (displayed == true)
        setToolTip(columnCount() - 1, mLine->logMode()->itemBuilder()->createToolTipText(mLine));
    else
        setToolTip(columnCount() - 1, QString());
}

bool LogViewWidgetItem::operator<(const QTreeWidgetItem &other) const
{
    int sortedColumn = treeWidget()->sortColumn();

    // If we sort items by date (always the first column)
    if (sortedColumn == 0) {
        const LogViewWidgetItem &otherItem = static_cast<const LogViewWidgetItem &>(other);
        return mLine->isOlderThan(*(otherItem.logLine()));
    }
    // Default sorting
    else {
        return text(sortedColumn) < other.text(sortedColumn);
    }
}
