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

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include <kurl.h>
#include <KLocalizedString>

#include "logFile.h"
#include "logLevel.h"

#include "globals.h"

#include "ksystemlogConfig.h"

LogModeConfiguration::LogModeConfiguration() {
	configuration = KSystemLogConfig::self();
}

LogModeConfiguration::~LogModeConfiguration() {
	//configuration is managed by KDE
}

//TODO Need a rewritting
/*
Reader* ReaderFactory::createReader(LogMode* logMode) {
	
	//Using Boot Mode in the current view
	else if (logMode==Globals::bootMode) {
		Reader* reader=new SystemReader(NULL, "boot_reader");
		return(reader);
	}
	
	//Using Authentication Mode in the current view
	else if (logMode==Globals::authenticationMode) {
		Reader* reader=new SystemReader(NULL, "authentication_reader");
		return(reader);
	}
	
	//Using Daemon Mode in the current view
	else if (logMode==Globals::daemonMode) {
		Reader* reader=new SystemReader(NULL, "daemon_reader");
		return(reader);
	}

	//Using Cups Mode in the current view
	else if (logMode==Globals::cupsMode) {
		Reader* reader=new CupsReader(NULL, "cups_reader");
		return(reader);
	}

	//Using Cups Access Mode in the current view
	else if (logMode==Globals::cupsAccessMode) {
		Reader* reader=new CupsAccessReader(NULL, "cups_access_reader");
		return(reader);
	}
	
	//Using Postfix Mode in the current view
	else if (logMode==Globals::postfixMode) {
		Reader* reader=new SystemReader(NULL, "postfix_reader");
		return(reader);
	}

	//Using Samba Mode in the current view
	else if (logMode==Globals::sambaMode) {
		Reader* reader=new SambaReader(NULL, "samba_reader");
		return(reader);
	}
	
	//Using SSH Mode in the current view
	else if (logMode==Globals::sshMode) {
		Reader* reader=new SSHReader(NULL, "ssh_reader");
		return(reader);
	}

	//Using X Session Mode in the current view
	else if (logMode==Globals::xsessionMode) {
		Reader* reader=new XSessionReader(NULL, "xsession_reader");
		return reader;
	}
	
	logError() << "LogMode not found : returns NULL Reader" << endl;
	return NULL;
}
*/

//TODO Move this method in LogModeFactory subclasses
/*
QList<LogFile> LogFilesFactory::createLogFiles(LogMode* logMode) {
	
	else if (logMode==Globals::instance()->bootMode()) {
		QList<LogFile> list;
		list.append(LogFilesFactory::instance()->getBootLogFile());
		return list;
	}

	else if (logMode==Globals::instance()->authenticationMode()) {
		QList<LogFile> list;
		list.append(LogFilesFactory::instance()->getAuthenticationLogFile());
		return list;
	}
	
	else if (logMode==Globals::instance()->daemonMode()) {
		return LogFilesFactory::getDaemonLogFiles();
	}

	else if (logMode==Globals::instance()->cupsMode()) {
		return LogFilesFactory::getCupsLogFiles();
	}
	
	else if (logMode==Globals::instance()->cupsAccessMode()) {
		return LogFilesFactory::getCupsAccessLogFiles();

	}
	
	else if (logMode==Globals::instance()->postfixMode()) {
		return LogFilesFactory::getPostfixLogFiles();
	}

	else if (logMode==Globals::instance()->sambaMode()) {
		return LogFilesFactory::getSambaLogFiles();
	}
	
	else if (logMode==Globals::instance()->sshMode()) {
		return LogFilesFactory::getSSHLogFiles();
	}

	
	else if (logMode==Globals::instance()->xsessionMode()) {
		return LogFilesFactory::getXSessionLogFiles();
	}
	
	logError() << "LogFiles not found : returns NULL Reader" << endl;
	
	return QList<LogFile>();

}


LogFile LogFilesFactory::getBootLogFile() {
	QString file=KSystemLogConfig::bootPath();
	return getGenericLogFile(file);
}

LogFile LogFilesFactory::getAuthenticationLogFile() {
	QString file=KSystemLogConfig::authenticationPath();
	return getGenericLogFile(file);
}

QList<LogFile> LogFilesFactory::getDaemonLogFiles() {
	QStringList files=KSystemLogConfig::daemonPaths();
	QList<int> levels=KSystemLogConfig::daemonLevels();
	return LogFilesFactory::getGenericLogFiles(files, levels);
}

QList<LogFile> LogFilesFactory::getCupsLogFiles() {
	QStringList stringList=KSystemLogConfig::cupsPaths();
	return getNoModeLogFiles(stringList);
}

QList<LogFile> LogFilesFactory::getCupsAccessLogFiles() {
	QStringList stringList=KSystemLogConfig::cupsAccessPaths();
	return getNoModeLogFiles(stringList);
}

QList<LogFile> LogFilesFactory::getPostfixLogFiles() {
	QStringList files=KSystemLogConfig::postfixPaths();
	QList<int> levels=KSystemLogConfig::postfixLevels();
	return LogFilesFactory::getGenericLogFiles(files, levels);
}

QList<LogFile> LogFilesFactory::getSambaLogFiles() {
	QStringList stringList=KSystemLogConfig::sambaPaths();
	return getNoModeLogFiles(stringList);
}

QList<LogFile> LogFilesFactory::getSSHLogFiles() {
	QStringList stringList=KSystemLogConfig::sshPaths();
	return getNoModeLogFiles(stringList);
}

QList<LogFile> LogFilesFactory::getXSessionLogFiles() {
	QStringList stringList=KSystemLogConfig::xSessionPaths();
	return getNoModeLogFiles(stringList);
}

*/


LogFile LogModeConfiguration::findGenericLogFile(const QString& file) {

	LogLevel* level=Globals::instance()->informationLogLevel();
	
	KUrl url(file);
	if (!url.isValid()) {
		logWarning() << i18n("URL '%1' is not valid, skipping this URL.", url.path()) << endl;
		return LogFile(KUrl(), Globals::instance()->noLogLevel());
	}
	
	return LogFile(url, level);
}

QList<LogFile> LogModeConfiguration::findGenericLogFiles(const QStringList& files) {
	QList<LogFile> logFiles;
	
	foreach(const QString &file, files) {
		logFiles.append(findGenericLogFile(file));
	}
	
	return logFiles;
}

QList<LogFile> LogModeConfiguration::findNoModeLogFiles(const QStringList& stringList) {

	QList<LogFile> logFiles;
	
	//Default level used for No Mode logs
	LogLevel* level=Globals::instance()->noLogLevel();
	
	foreach (const QString &string, stringList) {
		
		KUrl url(string);
		if (!url.isValid()) {
			logWarning() << i18n("URL '%1' is not valid, skipping this URL.", url.path()) << endl;
			continue;
		}
		
		logFiles.append(LogFile(url, level));

	}
	
	return logFiles;
}


