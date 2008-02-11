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

#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#include <QObject>
#include <QDataStream>
#include <QDebug>

#include <kurl.h>

class LogLevel;

class LogFilePrivate;

class LogFile : public QObject {
	
	Q_OBJECT
	
	public:
		LogFile();
		
		LogFile(const LogFile& logFile);
		LogFile(const KUrl& url, LogLevel* defaultLogLevel);
		
		virtual ~LogFile();

		bool operator==(const LogFile& other);
		
		LogFile& operator=(const LogFile& column);
		
		KUrl url() const;
		
		LogLevel* defaultLogLevel() const;

	private:
		LogFilePrivate* const d;
};

QDataStream & operator<< (QDataStream& out, const LogFile& column);
QDebug & operator<< (QDebug& out, const LogFile& column);

#endif // _LOG_FILE_H_
