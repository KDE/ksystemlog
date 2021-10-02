/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    if (displayed) {
        setToolTip(columnCount() - 1, mLine->logMode()->itemBuilder()->createToolTipText(mLine));
    } else {
        setToolTip(columnCount() - 1, QString());
    }
}

bool LogViewWidgetItem::operator<(const QTreeWidgetItem &other) const
{
    const int sortedColumn = treeWidget()->sortColumn();

    // If we sort items by date (always the first column)
    if (sortedColumn == 0) {
        const auto &otherItem = static_cast<const LogViewWidgetItem &>(other);
        return mLine->isOlderThan(*(otherItem.logLine()));
    }
    // Default sorting
    else {
        return text(sortedColumn) < other.text(sortedColumn);
    }
}
