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

#ifndef LOG_VIEW_WIDGET_H
#define LOG_VIEW_WIDGET_H

#include <QTreeWidget>
#include <QList>

#include "logViewColumns.h"

class LogViewWidgetItem;
class LogViewModel;
class LogLine;

class LogViewWidgetPrivate;

class LogViewWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit LogViewWidget(QWidget *parent = NULL);

    virtual ~LogViewWidget();
    void selectAll() Q_DECL_OVERRIDE;
    void setColumns(const LogViewColumns &columns);

    int itemCount() const;
    int notHiddenItemCount();

    QList<LogViewWidgetItem *> items();
    QList<LogLine *> logLines();

    /**
     * Return the related widget item of this logLine or NULL if it has not been found
     */
    LogViewWidgetItem *findItem(LogLine *logLine);

    LogViewWidgetItem *findNewestItem();

    LogViewModel *model() const;

    bool hasItemsSelected();
    LogViewWidgetItem *firstSelectedItem();
    LogViewWidgetItem *lastSelectedItem();

    void scrollToNewestItem();

    void expandAll();
    void collapseAll();

    void resizeColumns();

public Q_SLOTS:
    void toggleToolTip(bool enabled);

Q_SIGNALS:
    void columnsChanged(const LogViewColumns &columns);

private Q_SLOTS:
    void toggleHeader(QAction *action);

private:
    LogViewWidgetPrivate *const d;
};

#endif // LOG_VIEW_WIDGET_H
