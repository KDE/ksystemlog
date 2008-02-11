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

#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include <QList>
#include <QMutex>
#include <QString>
#include <QStringList>

#include <kurl.h>

#include "globals.h"

#include "logLine.h"

#include "logFile.h"
#include "logViewColumn.h"
#include "logViewColumns.h"

class LogViewModel;
class LogFileReader;
class LogMode;

class Analyzer : public QObject {
	
	Q_OBJECT
	
	public:
		enum ReadingMode {
				UpdatingRead,
				FullRead
		};
		Q_DECLARE_FLAGS(ReadingModes, ReadingMode)
		
		enum LogFileSortMode {
			AscendingSortedLogFile,
			FilteredLogFile,
			UnsortedLogFile
		};
		Q_DECLARE_FLAGS(LogFileSortModes, LogFileSortMode)

		explicit Analyzer(LogMode* logMode);
		
		~Analyzer();
		
		void watchLogFiles(bool enabled);
		

		void setLogFiles(const QList<LogFile>& logFiles);
		void setLogViewModel(LogViewModel* logViewModel);

		bool isParsingPaused() const;

		virtual LogViewColumns initColumns() = 0;

	public slots:
		void setParsingPaused(bool paused);
		
	protected:
		virtual LogFileReader* createLogFileReader(const LogFile& logFile) = 0;
		virtual Analyzer::LogFileSortMode logFileSortMode() = 0;
		
		virtual LogLine* parseMessage(const QString& logLine, const LogFile& originalFile) = 0;
		
	private:

		inline void informOpeningProgress(int currentPosition, int total);
		
		void deleteLogFiles();
		
		/**
		 * Parse and insert the buffered lines in the model
		 * Returns the count of inserted lines
		 */
		int insertLines(const QStringList& bufferedLines, const LogFile& logFile, ReadingMode readingMode);
		
		/**
		 * Parse and insert a line in the model
		 * Returns false if it was not inserted, true if it was
		 */
		bool insertLine(const QString& buffer, const LogFile& originalFile, ReadingMode readingMode);
		
	private slots:
		void logFileChanged(LogFileReader* logFileReader, Analyzer::ReadingMode readingMode, const QStringList& content);

	signals:
		void statusBarChanged(const QString& message);
		void errorOccured(const QString& title, const QString& message);
		
		void openingProgressed();
		
		void logUpdated(int lineTotal);
		
		void readFileStarted(const LogMode& logMode, const LogFile& logFile, int fileIndex, int fileCount);
		void readEnded();
		
	protected:
		//TODO Move those members to a D-pointer (and think about subclasses accesses)
		bool parsingPaused;
		
		LogViewModel* logViewModel;

		LogMode* logMode;
		
		QList<LogFileReader*> logFileReaders;
		
		QMutex* insertionLocking;
		
		long logLineInternalIdGenerator;

};

#endif
