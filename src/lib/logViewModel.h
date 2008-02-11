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

#ifndef _LOG_VIEW_MODEL_H_
#define _LOG_VIEW_MODEL_H_

#include <QObject>

#include "globals.h"
#include "analyzer.h"

class LogLine;
class LogViewWidget;

class LogViewModelPrivate;

class LogViewModel : public QObject {
	
	Q_OBJECT
	
	public:
		LogViewModel(LogViewWidget* logViewWidget);
		
		virtual ~LogViewModel();

		/**
		 * Clear the model
		 */
		void clear();
		
		bool insertNewLogLine(LogLine* line);

		int itemCount() const;
		bool isEmpty() const;
		
		bool isProcessingMultipleInsertions() const;
		
		void startingMultipleInsertions(Analyzer::ReadingMode readingMode);
		void endingMultipleInsertions(Analyzer::ReadingMode readingMode, int insertedLogLineCount);

		QList<LogLine*> logLines();
		
	signals:
		void processingMultipleInsertions(bool currentlyInserting);
		
	private:
		/**
		 * Prevent crossed multiple insertions between each LogFileReaders
		 */ 
		bool lockMultipleInsertions();
		
		void setFirstReadProcessed();

		bool logLineAlreadyExists(LogLine* line) const;
		
		bool isNewer(LogLine* line) const;
		
		/**
		 * Remove the oldest line
		 */
		void removeOldestLogLine();

		/**
		 * Insert this line
		 */
		void insert(LogLine* line);

		/**
		 * Remove recent status on previously new log lines
		 */
		void removeRecentStatusOfLogLines();
		
		LogViewModelPrivate* const d;

};

#endif //_LOG_VIEW_MODEL_H_
