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

#ifndef _XORG_ANALYZER_H_
#define _XORG_ANALYZER_H_

#include <KLocalizedString>

#include "analyzer.h"

#include "localLogFileReader.h"
#include "parsingHelper.h"
#include "xorgLogMode.h"


#define CONFIG_FILE_LOG_LEVEL_ICON "configure"
#define DEFAULT_SETTING_LOG_LEVEL_ICON "configure-toolbars"
#define COMMAND_LINE_LOG_LEVEL_ICON "konsole"
#define PROBED_LOG_LEVEL_ICON "favorites"
#define NOT_IMPLEMENTED_LOG_LEVEL_ICON "document-new"

class XorgAnalyzer : public Analyzer {
	Q_OBJECT

	public:
		XorgAnalyzer(LogMode* logMode) :
			Analyzer(logMode),
			currentDateTime(QDateTime::currentDateTime())
			{

			initializeTypeName();
		}

		virtual ~XorgAnalyzer() {

		}

		LogViewColumns initColumns() {
			LogViewColumns columns;

			columns.addColumn(LogViewColumn(i18n("Line"), false, false));
			columns.addColumn(LogViewColumn(i18n("Type"), false, false));
			columns.addColumn(LogViewColumn(i18n("Message"), false, false));

			columns.setGroupByDay(false);
			columns.setGroupByHour(false);

			return columns;
		}


	protected:
		LogFileReader* createLogFileReader(const LogFile& logFile) {
			return new LocalLogFileReader(logFile);
		}

		Analyzer::LogFileSortMode logFileSortMode() {
			return Analyzer::AscendingSortedLogFile;
		}

		LogLine* parseMessage(const QString& logLine, const LogFile& originalFile) {

			QString string(logLine);

			QString type;

			type=string.left(4);

			LogLevel* logLineType=findTypeName(type);

			//If the type is not empty, the log message has a type, so we can delete it
			if (logLineType!=NULL) {
				string=string.remove(0, 5);
			}
			else {
				logLineType=Globals::instance()->informationLogLevel();
			}

			QStringList list;
			list.append(logLineType->name());
			list.append(string);

			return new LogLine(
					logLineInternalIdGenerator++,
					currentDateTime,
					list,
					originalFile.url().path(),
					logLineType,
					logMode
			);
		}

	private:

		QMap<QString, LogLevel*> xorgLevels;

		void initializeTypeName() {
			xorgLevels[QLatin1String( "(--)" )]=new LogLevel(1001, i18n("Probed"),QLatin1String( PROBED_LOG_LEVEL_ICON ), QColor(246, 206, 30));
			xorgLevels[QLatin1String( "(**)" )]=new LogLevel(1002, i18n("From config file"),QLatin1String( CONFIG_FILE_LOG_LEVEL_ICON ), QColor(161, 133, 240));
			xorgLevels[QLatin1String( "(==)" )]=new LogLevel(1003, i18n("Default setting"),QLatin1String( DEFAULT_SETTING_LOG_LEVEL_ICON ), QColor(169, 189, 165));
			xorgLevels[QLatin1String( "(++)" )]=new LogLevel(1004, i18n("From command Line"),QLatin1String( COMMAND_LINE_LOG_LEVEL_ICON ), QColor(179, 181, 214));
			xorgLevels[QLatin1String( "(!!)" )]=Globals::instance()->noticeLogLevel();
			xorgLevels[QLatin1String( "(II)" )]=Globals::instance()->informationLogLevel();
			xorgLevels[QLatin1String( "(WW)" )]=Globals::instance()->warningLogLevel();
			xorgLevels[QLatin1String( "(EE)" )]=Globals::instance()->errorLogLevel();
			xorgLevels[QLatin1String( "(NI)" )]=new LogLevel(1005, i18n("Not implemented"),QLatin1String( NOT_IMPLEMENTED_LOG_LEVEL_ICON ), QColor(136, 146, 240));
			xorgLevels[QLatin1String( "(\?\?)" )]=Globals::instance()->noLogLevel();

		}

		LogLevel* findTypeName(const QString& type) {
			QMap<QString, LogLevel*>::iterator it;

			it=xorgLevels.find(type);
			if (it!=xorgLevels.end())
				return *it;
			else
				return NULL;

		}

		QDateTime currentDateTime;

};

#endif // _XORG_ANALYZER_H_
