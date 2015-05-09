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

#include "genericConfiguration.h"

#include <KLocalizedString>

#include "logging.h"
#include "defaults.h"

#include "globals.h"

#include "ksystemlogConfig.h"

class GenericLogModeConfigurationPrivate {
public:
	QStringList logFilesPaths;

	QList<int> logFilesLevels;

};

GenericLogModeConfiguration::GenericLogModeConfiguration(const QString& configurationGroup, const QStringList& defaultLogFilesPaths, const QList<int> defaultLogFilesLevels) :
	d(new GenericLogModeConfigurationPrivate()) {

  logDebug() << "Using Configuration Group : " << configurationGroup;
	configuration->setCurrentGroup(configurationGroup);

	configuration->addItemStringList(QLatin1String( "LogFilesPaths" ), d->logFilesPaths, defaultLogFilesPaths, QLatin1String( "LogFilesPaths" ));

	configuration->addItemIntList(QLatin1String( "LogFilesLevels" ), d->logFilesLevels, defaultLogFilesLevels, QLatin1String( "LogFilesLevels" ));

}

GenericLogModeConfiguration::~GenericLogModeConfiguration() {
	delete d;
}


QStringList GenericLogModeConfiguration::logFilesPaths() const {
	return d->logFilesPaths;
}

QList<int> GenericLogModeConfiguration::logFilesLevels() const {
	return d->logFilesLevels;
}

void GenericLogModeConfiguration::setLogFilesPaths(const QStringList& logFilesPaths) {
	d->logFilesPaths = logFilesPaths;
}

void GenericLogModeConfiguration::setLogFilesLevels(const QList<int>& logFilesLevels) {
	d->logFilesLevels = logFilesLevels;
}


QList<LogFile> GenericLogModeConfiguration::findGenericLogFiles() {

	QList<LogFile> logFiles;

	if (d->logFilesPaths.size() != d->logFilesLevels.size()) {
    logDebug() << i18n("The two arrays size are different, skipping the reading of log files.");
		return logFiles;
	}

	LogLevel* level;

	QListIterator<QString> itString(d->logFilesPaths);
	QListIterator<int> itInt(d->logFilesLevels);

	while (itString.hasNext()) {
		int intValue=itInt.next();
		QString stringValue=itString.next();

		if (intValue>=0 && intValue<(int) Globals::instance()->logLevels().count())
			level=Globals::instance()->logLevels().at(intValue);
		else
			level=Globals::instance()->informationLogLevel();

		QUrl url(stringValue);
		if (!url.isValid()) {
      logWarning() << i18n("URL '%1' is not valid, skipping this URL.", url.path());
			continue;
		}

		logFiles.append(LogFile(url, level));

	}

	return logFiles;
}



