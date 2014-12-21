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

#include "logLevelFileList.h"

#include <QString>
#include <QStringList>
#include <QListWidgetItem>

#include <QPushButton>

// KDE includes
#include <QMenu>
#include <KLocalizedString>
#include <kfiledialog.h>
#include <kdialog.h>
#include <kmessagebox.h>
#include <QIcon>
#include <kurl.h>

#include <klistwidget.h>

#include "defaults.h"

#include "logging.h"
#include "globals.h"

#include "logLevel.h"
#include "logLevelSelectionDialog.h"

int LogLevelFileList::LogLevelRole = 33;

LogLevelFileList::LogLevelFileList(QWidget* parent, const QString& description) :
	FileList(parent, description)
	{

	logDebug() << "Initializing specific file list..." << endl;

	changeItem=new QPushButton(i18n("&Change Status..."));
	changeItem->setToolTip(i18n("Change the level of the current file(s)"));
	changeItem->setWhatsThis(i18n("Changes the level of the current file(s). See KSystemLog documentation for more information about each log level."));
	
	//Insert the button just after the "Modify File" button
	buttonsLayout()->insertWidget(2, changeItem);
	
	QAction* action = fileListHelper.prepareButtonAndAction(changeItem, QIcon::fromTheme( QLatin1String( "favorites" )), this, SLOT(changeItemType()));

	//Insert the action just after the "Modify File" action
	fileList->insertAction(fileList->actions().at(2), action);
	
	changeItem->setEnabled(false);
	
	connect(fileList, SIGNAL(itemSelectionChanged()), this, SLOT(updateSpecificButtons()));
	connect(this, SIGNAL(fileListChanged()), this, SLOT(updateSpecificButtons()));
	
	disconnect(fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(modifyItem(QListWidgetItem*)));
	connect(fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(changeItemType()));

	updateSpecificButtons();
	
	logDebug() << "Specific file list initialized" << endl;

}
 

LogLevelFileList::~LogLevelFileList() {
	//changeItem is managed automatically
}

void LogLevelFileList::insertItem(LogLevel* level, const QString& itemText) {
	QListWidgetItem* item = new QListWidgetItem(QIcon(level->pixmap()), itemText, fileList);
	item->setData(LogLevelFileList::LogLevelRole, level->id());
}

void LogLevelFileList::addItem() {
	//Open a standard Filedialog
	QList<QUrl> urls=fileListHelper.openUrls();
	
	QStringList paths=fileListHelper.findPaths(urls);
	foreach(const QString &path, paths) {
		insertItem(Globals::instance()->informationLogLevel(), path);
	}

	emit fileListChanged();
}

void LogLevelFileList::updateSpecificButtons() {
	if (fileList->selectedItems().count()>0)
		changeItem->setEnabled(true);
	else 
		changeItem->setEnabled(false);

}

void LogLevelFileList::changeItemType() {
	logDebug() << "Changing item type..." << endl;

	LogLevelSelectionDialog logLevelSelectionDialog(this);
	
	foreach (LogLevel* level, Globals::instance()->logLevels()) {
		logLevelSelectionDialog.logLevels()->addItem(new QListWidgetItem(QIcon(level->pixmap()), level->name()));
	}
	
	int choice=logLevelSelectionDialog.exec();
	
	if (choice==QDialog::Accepted) {

		QList<QListWidgetItem*> selectedLogLevels = logLevelSelectionDialog.logLevels()->selectedItems(); 
		if (selectedLogLevels.isEmpty()==false) {
			QListWidgetItem* logLevel = selectedLogLevels.at(0);
			int selectedLogLevel=logLevelSelectionDialog.logLevels()->row(logLevel);
			
			
			QList<QListWidgetItem*> selectedItems = fileList->selectedItems();
			foreach (QListWidgetItem* item, selectedItems) {
				item->setIcon(logLevel->icon());
				item->setData(LogLevelFileList::LogLevelRole, selectedLogLevel);
			}
			
			emit fileListChanged();
		}
		
	}

}


LogLevel* LogLevelFileList::level(int i) {
	return Globals::instance()->logLevels().at( fileList->item(i)->data(LogLevelFileList::LogLevelRole).toInt() );
}


QList<int> LogLevelFileList::levels() {
	QList<int> levels;
	int count=fileList->count();
		
	for (int i=0; i<count; i++) {
		levels.append(this->level(i)->id());
	}
	
	return levels;

}

void LogLevelFileList::addPaths(const QStringList& stringList, const QList<int>& valueList) {
	//A little security test
	if (stringList.size() != valueList.size()) {
		logDebug() << i18n("The two arrays size are different, skipping the reading of generic paths.") << endl;
		return;
	}
	
	QListIterator<QString> itString(stringList);
	QListIterator<int> itInt=(valueList);
	
	while (itString.hasNext()) {
		int valueInt = itInt.next();
		QString valueString = itString.next();
		
		LogLevel* level;
		if (valueInt>=0 && valueInt<(int) Globals::instance()->logLevels().count())
			level=Globals::instance()->logLevels().at(valueInt);
		else
			level=Globals::instance()->informationLogLevel();
		
		insertItem(level, valueString);
	
	}

	emit fileListChanged();
}



