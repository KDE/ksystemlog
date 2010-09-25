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

#ifndef _CUPS_PDF_ANALYZER_H_
#define _CUPS_PDF_ANALYZER_H_


#include <klocale.h>

#include "analyzer.h"

#include "localLogFileReader.h"
#include "logging.h"
#include "parsingHelper.h"

#include "cupsPdfLogMode.h"

class CupsPdfAnalyzer : public Analyzer {

	Q_OBJECT

	public:
		//Fri Sep 30 21:58:37 2005  [ERROR] failed to create spool directory (/var/spool/cups-pdf/SPOOL)
		explicit CupsPdfAnalyzer(LogMode* logMode) :
			Analyzer(logMode),
			cupsPdfRegex(QLatin1String( "\\S* " ) + ParsingHelper::instance()->syslogDateTimeRegexp() + QLatin1String( "[ ]+\\[(\\w*)\\][ ]+(.*)" )) { // \\[(.*)\\] (\\S*) (\\S*) (\\S*)
		}

		virtual ~CupsPdfAnalyzer() {

		}

		LogViewColumns initColumns() {
			LogViewColumns columns;

			columns.addColumn(LogViewColumn(i18n("Date"), true, false));
			columns.addColumn(LogViewColumn(i18n("Message"), true, false));

			return columns;
		}


	protected:

		QRegExp cupsPdfRegex;

		LogFileReader* createLogFileReader(const LogFile& logFile) {
			return new LocalLogFileReader(logFile);
		}

		Analyzer::LogFileSortMode logFileSortMode() {
			return Analyzer::AscendingSortedLogFile;
		}

		/*
		 * http://www.physik.uni-wuerzburg.de/~vrbehr/cups-pdf/documentation.shtml (cups-pdf_log)
		 *
		 * Thu Jun 14 12:40:35 2007 [STATUS] identification string sent
		 * Thu Jun 14 12:43:07 2007 [ERROR] failed to set file mode for PDF file (non fatal) (/var/spool/cups-pdf/root/Test_Pdf.pdf)
		 * Thu Jun 14 12:43:07 2007 [STATUS] PDF creation successfully finished (root)
		 * Fri Sep 30 21:58:37 2005  [ERROR] failed to create spool directory (/var/spool/cups-pdf/SPOOL)
		 * Sat Oct  1 09:11:45 2005  [ERROR] failed to create spool directory (/var/spool/cups-pdf/SPOOL)
		 */
		LogLine* parseMessage(const QString& logLine, const LogFile& originalLogFile) {

			QString line(logLine);

			int firstPosition = cupsPdfRegex.indexIn(logLine);
			if (firstPosition == -1) {
				logDebug() << "Unable to parse line " << logLine << endl;
				return NULL;
			}

			QStringList capturedTexts = cupsPdfRegex.capturedTexts();

			/*
			logDebug() << "------------------------------------------" << endl;
			foreach(QString cap, capturedTexts) {
				logDebug() << cap << endl;
			}
			logDebug() << "------------------------------------------" << endl;
			*/

			//Remove full line
			capturedTexts.removeAt(0);

			QDateTime dateTime=ParsingHelper::instance()->parseSyslogDateTime(capturedTexts.takeAt(0));
			LogLevel* logLevel = findLogLevel(capturedTexts.takeAt(0));

			return new LogLine(
					logLineInternalIdGenerator++,
					dateTime,
					capturedTexts,
					originalLogFile.url().path(),
					logLevel,
					logMode
			);
		}

		LogLevel* findLogLevel(const QString& level) {
			if ( level == QLatin1String( "ERROR" ))
				return Globals::instance()->errorLogLevel();

			//level == "STATUS"
			return Globals::instance()->informationLogLevel();
		}
};

#endif // _CUPS_PDF_ANALYZER_H_
