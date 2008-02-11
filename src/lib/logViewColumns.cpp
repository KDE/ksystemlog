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

#include "logViewColumns.h"

#include <QStringList>
#include <QList>

#include "logging.h"
#include <kurl.h>

#include "globals.h"
#include "logViewColumn.h"

class LogViewColumnsPrivate {
public:

	QList<LogViewColumn> columns;
	bool groupByLogLevel;
	bool groupByDay;
	bool groupByHour;
	bool groupByLogFile;

};

LogViewColumns::LogViewColumns() :
	d(new LogViewColumnsPrivate()) {
	
	d->groupByLogLevel = true;
	d->groupByDay = true;
	d->groupByHour = true;
	d->groupByLogFile = true;


}
	
LogViewColumns::~LogViewColumns() {
	delete d;
}

void LogViewColumns::setGroupByLogLevel(bool value) {
	d->groupByLogLevel=value;
}

void LogViewColumns::setGroupByDay(bool value) {
	d->groupByDay=value;
}

void LogViewColumns::setGroupByHour(bool value) {
	d->groupByHour=value;
}

void LogViewColumns::setGroupByLogFile(bool value) {
	d->groupByLogFile=value;
}


bool LogViewColumns::isGroupByLogLevel() const {
	return d->groupByLogLevel;
}

bool LogViewColumns::isGroupByDay() const {
	return d->groupByDay;
}

bool LogViewColumns::isGroupByHour() const {
	return d->groupByHour;
}

bool LogViewColumns::isGroupByLogFile() const {
	return d->groupByLogFile;
}

QStringList LogViewColumns::toStringList() const {
	QStringList columnNames;
	
	foreach (const LogViewColumn& column, d->columns) {
		columnNames.append(column.columnName());
	}
		
	return columnNames;
}

void LogViewColumns::addColumn(const LogViewColumn& column) {
	d->columns.append(column);
}

QList<LogViewColumn> LogViewColumns::columns() const {
	return d->columns;
}

LogViewColumns& LogViewColumns::operator=(const LogViewColumns& columns) {
	d->columns = columns.columns();
	d->groupByLogLevel = columns.isGroupByLogLevel();
	d->groupByDay = columns.isGroupByDay();
	d->groupByHour = columns.isGroupByHour();
	d->groupByLogFile = columns.isGroupByLogFile();

	return *this;
}

QDataStream & operator<< (QDataStream& out, const LogViewColumns& columns) {
	out << columns.columns();
	return out;
}
QDebug & operator<< (QDebug& out, const LogViewColumns& columns) {
	out << columns.columns();
	return out;
}
