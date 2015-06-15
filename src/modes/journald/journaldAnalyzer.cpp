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

#include "journaldAnalyzer.h"

//#include <QStringList>
//#include <QDateTime>

#include <KLocalizedString>

//#include "globals.h"
//#include "logging.h"

//#include "logLine.h"
//#include "logMode.h"
//#include "logLevel.h"
//#include "logViewWidget.h"

//#include "logViewModel.h"

//#include "parsingHelper.h"

//#include "ksystemlogConfig.h"

JournaldAnalyzer::JournaldAnalyzer(LogMode *logMode)
    : Analyzer(logMode)
{
}

JournaldAnalyzer::~JournaldAnalyzer()
{
}

LogViewColumns JournaldAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, true));
    columns.addColumn(LogViewColumn(i18n("Unit"), true, true));
    columns.addColumn(LogViewColumn(i18n("Message"), true, true));
    return columns;
}

void JournaldAnalyzer::setLogFiles(const QList<LogFile> &logFiles)
{
}

void JournaldAnalyzer::watchLogFiles(bool enabled)
{
}
