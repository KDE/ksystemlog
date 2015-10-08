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

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "parsingHelper.h"
#include "xorgLogMode.h"

#define CONFIG_FILE_LOG_LEVEL_ICON "configure"
#define DEFAULT_SETTING_LOG_LEVEL_ICON "configure-toolbars"
#define COMMAND_LINE_LOG_LEVEL_ICON "konsole"
#define PROBED_LOG_LEVEL_ICON "favorites"
#define NOT_IMPLEMENTED_LOG_LEVEL_ICON "document-new"

class XorgAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    XorgAnalyzer(LogMode *logMode);

    virtual ~XorgAnalyzer();

    LogViewColumns initColumns();

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile);

    Analyzer::LogFileSortMode logFileSortMode();

    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile);

private:
    QMap<QString, LogLevel *> xorgLevels;

    void initializeTypeName();

    LogLevel *findTypeName(const QString &type);

    QDateTime currentDateTime;
};

#endif // _XORG_ANALYZER_H_
