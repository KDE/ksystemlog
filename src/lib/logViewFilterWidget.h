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

#ifndef LOG_VIEW_FILTER_WIDGET_H
#define LOG_VIEW_FILTER_WIDGET_H

#include <QWidget>

#include <ktreewidgetsearchline.h>

#include "logViewColumns.h"

class LogViewWidget;
class LogViewWidgetSearchLine;
class KComboBox;

class LogViewFilterWidgetPrivate;

class LogViewFilterWidget : public QWidget {

	Q_OBJECT

public:
	LogViewFilterWidget();

	~LogViewFilterWidget();
	
  KComboBox* filterList();
  LogViewWidgetSearchLine* filterLine();

public slots:
	void updateFilterColumns(const LogViewColumns& list);

private slots:
	void changeColumnFilter(int column);

signals:
	void treeWidgetUpdated();

private:
	void initSearchListFilter();

	LogViewFilterWidgetPrivate* const d;


};


class LogViewWidgetSearchLine : public KTreeWidgetSearchLine {

	Q_OBJECT

public:
	LogViewWidgetSearchLine();

	~LogViewWidgetSearchLine();
	
public:
  // Silence compiler warning
  using KTreeWidgetSearchLine::updateSearch;

  // Reimplemented just to send a signal _AFTER_ the tree updating
	void updateSearch(const QString& pattern = QString());

signals:
	void treeWidgetUpdated();

};

#endif //LOG_VIEW_FILTER_WIDGET_H
