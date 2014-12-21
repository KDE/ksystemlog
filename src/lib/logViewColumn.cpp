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


#include "logViewColumn.h"


#include <QString>
#include <QDataStream>


#include <kurl.h>
#include <kiconloader.h>

class LogViewColumnPrivate {
public:
	QString columnName;
	
	bool filtered;
	bool grouped;

};

LogViewColumn::LogViewColumn(const QString& name, bool filtered, bool grouped) :
	d(new LogViewColumnPrivate()) {
	
	d->columnName = name;
	d->filtered = filtered;
	d->grouped = grouped;
}

LogViewColumn::LogViewColumn(const LogViewColumn& column) :
	d(new LogViewColumnPrivate()) {
	
	d->columnName = column.columnName();
	d->filtered = column.isFiltered();
	d->grouped = column.isGrouped();
}

LogViewColumn::~LogViewColumn() {
	delete d;
}

QString LogViewColumn::columnName() const {
	return d->columnName;
}

bool LogViewColumn::isGrouped() const {
	return d->grouped;
}
bool LogViewColumn::isFiltered() const {
	return d->filtered;
}

LogViewColumn& LogViewColumn::operator=(const LogViewColumn& column) {
	d->columnName = column.columnName();
	d->grouped = column.isGrouped();
	d->filtered = column.isFiltered();
	
	return *this;
}

QDataStream& operator<< (QDataStream& out, const LogViewColumn& column) {
	out << column.columnName();
	return out;
}

QDebug& operator<< (QDebug& out, const LogViewColumn& column) {
	out << column.columnName();
	return out;
}
