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

#ifndef LOG_LINE_H
#define LOG_LINE_H

#include <QStringList>
#include <QDateTime>

#include "globals.h"

#include "logLevel.h"

class LogViewWidget;
class QDateTime;
class QStringList;
class LogViewWidgetItem;

class LogLinePrivate;

class LogLine {
	
	public:
		LogLine(
				long internalId,
				const QDateTime& dateTime, 
				const QStringList& logItems, 
				const QString& originalFile, 
				LogLevel* level, 
				LogMode* logMode
		);

		virtual ~LogLine();
		
		bool isOlderThan(const LogLine& other) const;
		bool isNewerThan(const LogLine& other) const;
				
		bool equals(const LogLine& other) const;

		bool isSameTime(const LogLine& other) const;

		LogLevel* logLevel() const;
		
		long internalId() const;
		QDateTime time() const;
		QStringList logItems() const;
		QString sourceFileName() const;
		
		LogMode* logMode() const;

		bool itemExists() const;

		void setLogItems(const QStringList& logItems);
		void setLogLevel(LogLevel* level);
		void setLogMode(LogMode* logMode);
		
		void setRecent(bool recent);
		void setItem(LogViewWidgetItem* item);

		QString formattedText();
		
		QString exportToText() const;

	protected:
		LogLinePrivate* const d;
		
	private:
		void initializeItem();

};

#endif
