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

#include "openLogMode.h"

#include <QString>
#include <QList>
#include <QFileDialog>

#include <KLocalizedString>

#include <ktoggleaction.h>
#include <KLocalizedString>
#include <kmessagebox.h>

#include "logging.h"
#include "logMode.h"

#include "logModeItemBuilder.h"
#include "openAnalyzer.h"

OpenLogMode::OpenLogMode(QWidget* parent) :
	LogMode(QLatin1String( OPEN_LOG_MODE_ID ), i18n("Log File"),QLatin1String( OPEN_MODE_ICON )),
	parent(parent) {

	d->logModeConfiguration = NULL;

	d->logModeConfigurationWidget = NULL;

	d->itemBuilder = new LogModeItemBuilder();

	d->action = NULL;

}

OpenLogMode::~OpenLogMode() {

}

Analyzer* OpenLogMode::createAnalyzer() {
	return new OpenAnalyzer(this);
}

QList<LogFile> OpenLogMode::createLogFiles() {
	//Open a standard Filedialog
  QUrl openingFileName(QFileDialog::getOpenFileUrl(parent, i18n("Open Location"), QUrl(), QString()));
  logDebug() << "Opening file : " << openingFileName.url();

	if (openingFileName.isEmpty()) {
		return QList<LogFile>();
	}

	if (openingFileName.isValid()) {
		LogFile logFile(openingFileName, Globals::instance()->informationLogLevel());
		QList<LogFile> logFiles;
		logFiles.append(logFile);

		return logFiles;

	}

	QString message(i18n("Malformed URL. Unable to open this file."));
	KMessageBox::error(parent, message, i18n("Unable to open this file."), KMessageBox::Notify);

	return QList<LogFile>();
}
