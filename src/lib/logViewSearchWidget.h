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

#ifndef _LOG_VIEW_SEARCH_WIDGET_H_
#define _LOG_VIEW_SEARCH_WIDGET_H_

#include <QWidget>
#include <QTreeWidgetItemIterator>

#include "ui_logViewSearchWidgetBase.h"

class LogViewWidget;
class LogViewWidgetItem;

class LogViewSearchWidgetPrivate;

class LogViewSearchWidget : public QWidget, public Ui::LogViewSearchWidgetBase
{
    Q_OBJECT

public:
    LogViewSearchWidget();

    ~LogViewSearchWidget();

    void setTreeWidget(LogViewWidget *logViewWidget);

public Q_SLOTS:
    void displaySearch();

    void findFirst();

    void findFirst(const QString &);
    void findNext();
    void findPrevious();

private Q_SLOTS:
    void hideMessage();

    void highlightAll();

private:
    enum Direction { Next, Previous };
    Q_DECLARE_FLAGS(Directions, Direction)

    void showMessage(const QString &text, const QString &iconText);

    void internalFind(LogViewWidgetItem *fromItem, Direction direction);

    void setSearchLineNotFound(bool notFound);
    void unselectAll();
    bool findItem(LogViewWidgetItem *item);

    bool compareItem(LogViewWidgetItem *item);

    void unlightAll();

    inline void initIterator(QTreeWidgetItemIterator &it, Direction direction);
    inline void iteratorJump(QTreeWidgetItemIterator &it, Direction direction);

    LogViewSearchWidgetPrivate *const d;
};

#endif //_LOG_VIEW_SEARCH_WIDGET_H_
