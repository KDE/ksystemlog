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

#include "loggerDialog.h"

#include <QProcess>

#include <klocale.h>
#include <kmessagebox.h>

//Project includes
#include "logging.h"

#include "logLevel.h"
#include "globals.h"

class LoggerDialogPrivate {
public:
	QMap<QString, QString> facilities;
	
	QMap<QString, QString> priorities;
	QMap<QString, QPixmap> priorityIcons;

};

LoggerDialog::LoggerDialog(QWidget *parent) :
	QDialog(parent),
	d(new LoggerDialogPrivate()) {
	
	setupUi(this);
	
	connect(buttonOK, SIGNAL(clicked()), this, SLOT(sendMessage()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(hide()));
	
	connect(tagActivation, SIGNAL(toggled(bool)), this, SLOT(changeTagActivation(bool)));
	connect(fileActivation, SIGNAL(toggled(bool)), this, SLOT(changeFileActivation(bool)));
	connect(messageActivation, SIGNAL(toggled(bool)), this, SLOT(changeMessageActivation(bool)));
	
	
	connect(file, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged()));
	connect(message, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged()));
	connect(tag, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged()));

	buildMaps();
	
	//Fill the priority ComboBox
	QList<QString> prioKeys(d->priorities.keys());
	
	QList<QString>::Iterator itPriority;
	for (itPriority=prioKeys.begin(); itPriority!=prioKeys.end(); ++itPriority) {
		priority->addItem(d->priorityIcons[*itPriority], *itPriority);
	}

	
	//Select the right priority
	for (int i=0; i<priority->count(); ++i) {
		if (priority->itemText(i)==Globals::instance()->noticeLogLevel()->name()) {
			priority->setCurrentIndex(i);
			break;
		}	
	}

	//Fill the priority ComboBox
	QList<QString> keys(d->facilities.keys());
	
	QList<QString>::Iterator itFacility;
	for (itFacility=keys.begin(); itFacility!=keys.end(); ++itFacility) {
		facility->addItem(*itFacility);
	}

	//Select the right facility
	for (int i=0; i<facility->count(); ++i) {
		if (facility->itemText(i)==i18n("User")) {
			facility->setCurrentIndex(i);
			break;
		}
	}
        tag->setEnabled(false);

}

LoggerDialog::~LoggerDialog() {
	delete d;
}

void LoggerDialog::initialize() {
	logDebug() << "Initializing Logger dialog..." << endl;

	message->clear();
	message->setFocus();
}

void LoggerDialog::buildMaps() {

	//Fill the facility map
	d->facilities[i18n("Authentication")]="auth";
	d->facilities[i18n("Private Authentication")]="authpriv";
	d->facilities[i18n("Cron")]="cron";
	d->facilities[i18n("Daemon")]="daemon";
	d->facilities[i18n("FTP")]="ftp";
	d->facilities[i18n("Kernel")]="kern";
	d->facilities[i18n("LPR")]="lpr";
	d->facilities[i18n("Mail")]="mail";
	d->facilities[i18n("News")]="news";
	d->facilities[i18n("Syslog")]="syslog";
	d->facilities[i18n("User")]="user";
	d->facilities[i18n("UUCP")]="uucp";
	
	d->facilities[i18n("Local 0")]="local0";
	d->facilities[i18n("Local 1")]="local1";
	d->facilities[i18n("Local 2")]="local2";
	d->facilities[i18n("Local 3")]="local3";
	d->facilities[i18n("Local 4")]="local4";
	d->facilities[i18n("Local 5")]="local5";
	d->facilities[i18n("Local 6")]="local6";
	d->facilities[i18n("Local 7")]="local7";

	//Fill the priority map
	d->priorities[Globals::instance()->debugLogLevel()->name()]="debug";
	d->priorities[Globals::instance()->informationLogLevel()->name()]="info";
	d->priorities[Globals::instance()->noticeLogLevel()->name()]="notice";
	d->priorities[Globals::instance()->warningLogLevel()->name()]="warning";
	d->priorities[Globals::instance()->errorLogLevel()->name()]="err";
	d->priorities[Globals::instance()->criticalLogLevel()->name()]="crit";
	d->priorities[Globals::instance()->alertLogLevel()->name()]="alert";
	d->priorities[Globals::instance()->emergencyLogLevel()->name()]="emerg";
	
	//Fill the priority icon map
	d->priorityIcons[Globals::instance()->debugLogLevel()->name()]=Globals::instance()->debugLogLevel()->icon();
	d->priorityIcons[Globals::instance()->informationLogLevel()->name()]=Globals::instance()->informationLogLevel()->icon();
	d->priorityIcons[Globals::instance()->noticeLogLevel()->name()]=Globals::instance()->noticeLogLevel()->icon();
	d->priorityIcons[Globals::instance()->warningLogLevel()->name()]=Globals::instance()->warningLogLevel()->icon();
	d->priorityIcons[Globals::instance()->errorLogLevel()->name()]=Globals::instance()->errorLogLevel()->icon();
	d->priorityIcons[Globals::instance()->criticalLogLevel()->name()]=Globals::instance()->criticalLogLevel()->icon();
	d->priorityIcons[Globals::instance()->alertLogLevel()->name()]=Globals::instance()->alertLogLevel()->icon();
	d->priorityIcons[Globals::instance()->emergencyLogLevel()->name()]=Globals::instance()->emergencyLogLevel()->icon();

}

void LoggerDialog::textChanged() {
	if (fileActivation->isChecked() && file->url().isEmpty()) {
		buttonOK->setEnabled(false);
		return;
	}

	if (tagActivation->isChecked() && tag->text().isEmpty()) {
		buttonOK->setEnabled(false);
		return;
	}
	
	if (messageActivation->isChecked() && message->text().isEmpty()) {
		buttonOK->setEnabled(false);
		return;
	}

	buttonOK->setEnabled(true);

}

void LoggerDialog::changeTagActivation(bool activation) {
	tag->setEnabled(activation);
	
	textChanged();
}

void LoggerDialog::changeFileActivation(bool activation) {
	file->setEnabled(activation);
	
	textChanged();
}

void LoggerDialog::changeMessageActivation(bool activation) {
	message->setEnabled(activation);
	
	textChanged();
}


void LoggerDialog::sendMessage() {
	
	QProcess process;

	QStringList arguments;
	
	if (useProcessIdentifier->isChecked()) {
		arguments << "-i";
	}
	
	if (tagActivation->isChecked()) {
		arguments << "-t";
		
		arguments << tag->text();
	}

	QString prioritySelected=priority->currentText();
	
	if (prioritySelected!=Globals::instance()->noLogLevel()->name()) {
		arguments << "-p";
		
		QString p(d->facilities[facility->currentText()]);
		p+='.';
		p+=d->priorities[priority->currentText()];
		
		arguments << p;
	}

	//If we read the content of a file
	if (fileActivation->isChecked()) {
		arguments << "-f";
		
		arguments << file->url().path();
	}
	//Else, the user types the content of its message
	else {
		//Remove bad "\n" characters
		arguments << message->text().replace("\n", " ");
	}

	// QProcess::Block, QProcess::Stdout 
	process.start("logger", arguments);
	
	//If the launching of the command failed
	if (process.error() == QProcess::FailedToStart) {
		KMessageBox::error(this, i18n("Unable to find the 'logger' command on your system. Please type 'logger' in Konsole to be sure that this command is not installed."), i18n("Command not found"));
		return;
	}
	
	if (process.exitStatus() == QProcess::CrashExit) {
		KMessageBox::error(this, i18n("The 'logger' command has not been properly exited."), i18n("Execution problem"));
		return;
	}

	//No such file or directory
	if (process.exitCode()==1) {
		KMessageBox::error(this, i18n("This file does not exist. Please choose a right file."), i18n("File not valid"));
		return;
	}

	//Hide the Logger Dialog
	hide();
}


#include "loggerDialog.moc"
