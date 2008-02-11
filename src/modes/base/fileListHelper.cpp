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

#include "fileListHelper.h"

#include <QObject>
#include <QAction>
#include <QPushButton>

#include <klocale.h>
#include <kfiledialog.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <kiconloader.h>

#include "defaults.h"

#include "logging.h"

FileListHelper::FileListHelper(QWidget* p) :
	parent(p) {
	
}

FileListHelper::~FileListHelper() {

}

QAction* FileListHelper::prepareButtonAndAction(QPushButton* button, const QIcon& icon) {
	//Initialize action
	QAction* action = new QAction(button);
	action->setIcon(icon);
	action->setText(button->text());
	
	//Initialize button
	button->setIcon(icon);
	//Allow the disabling of the matching action when disabling this button
	button->addAction(action);
	
	//Assert that when an action is triggered, the related button sends a clicked() event
	//(the button is the QObject which is connected to custom slots)
	connect(action, SIGNAL(triggered(bool)), button, SLOT(click()));
	
	return action;
}

QAction* FileListHelper::prepareButtonAndAction(QPushButton* button, const QIcon& icon, const QObject* receiver, const char* member) {
	QAction* action = prepareButtonAndAction(button, icon);
	connect(button, SIGNAL(clicked(bool)), receiver, member);
	
	return action;
}

void FileListHelper::prepareButton(QPushButton* button, const QIcon& icon, const QObject* receiver, const char* member, QWidget* fileList) {
	//Initialize action
	QAction* action = prepareButtonAndAction(button, icon, receiver, member);
	fileList->addAction(action);
}

QStringList FileListHelper::findPaths(KUrl::List urls) {
	QStringList paths;
	
	for (KUrl::List::iterator it=urls.begin(); it!=urls.end(); ++it) {
		KUrl url(*it);
		
		if (isValidFile(url)) {
			
			//If this Url uses a joker (i.e. : "/var/log/apache2/*")
			if (url.fileName().contains("*")) {
				QStringList foundPaths = expandJoker(url.path());
				logDebug() << "Found paths of " << url.path() << ":" << foundPaths << endl;
				foreach(const QString &foundPath, foundPaths) {
					paths.append(foundPath);
				}
			}
			else {
				paths.append(url.path());
			}
		}
	}
	
	return paths;
}

bool FileListHelper::isValidFile(const KUrl& url) {
	QString message;
	
	//If it is not valid
	if (!url.isValid()) {
		return false;
	}

	//If it is not a local file
	if (!url.isLocalFile()) {
		message=i18n("'%1' is not a local file.", url.path());
		KMessageBox::error(parent, message, i18n("File selection failed"), KMessageBox::Notify);
		return false;
	}
	
	//If it's a directory, it's not valid
	if (QDir(url.path()).exists()) {
		return false;
	}
		
	return true;
}

KUrl::List FileListHelper::openUrls() {
	return KFileDialog::getOpenUrls(KUrl(DEFAULT_LOG_FOLDER), "*|" + i18n("All Files (*)") + "\n*.log|" + i18n("Log Files (*.log)"), parent, i18n("Choose Log File"));
}

KUrl FileListHelper::openUrl(const QString& originPath) {
	return KFileDialog::getOpenUrl(KUrl(originPath), "*|" + i18n("All Files (*)") + "\n*.log|" + i18n("Log Files (*.log)"), parent, i18n("Choose Log File"));
}

QStringList FileListHelper::expandJoker(const KUrl& url) {
	QDir directory = QDir(url.path().left(url.path().count() - url.fileName().count()));
	
	logDebug() << "Dir " << directory.path() << endl;
	QString filename = url.fileName();
	
	if (filename.isEmpty()) {
		return QStringList();
	}
	
	QStringList foundPaths;
	QStringList files = directory.entryList(QStringList(filename), QDir::Files | QDir::NoSymLinks);
	foreach(const QString &file, files) {
		foundPaths.append(directory.absoluteFilePath(file));
	}
	
	return foundPaths;
}

void FileListHelper::setEnabledAction(QPushButton* button, bool enabled) {
	button->setEnabled(enabled);
	
	QList<QAction*> actions = button->actions();
	foreach (QAction* action, actions) {
		action->setEnabled(enabled);
	}
}

#include "fileListHelper.moc"
