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

#include "tabLogViewsWidget.h"

#include <QPushButton>
#include <QList>
#include <QMenu>

#include <KLocalizedString>
#include <kiconloader.h>
#include <QIcon>

#include "logging.h"

#include "view.h"
#include "logViewExport.h"

#include "logMode.h"
#include "defaults.h"
#include "logManager.h"
#include "tabLogManager.h"
#include "logViewWidget.h"

class TabLogViewsWidgetPrivate {
public:
	QList<TabLogManager*> tabLogManagers;

	QMenu* contextMenu;
};

TabLogViewsWidget::TabLogViewsWidget(QWidget* parent) :
  QTabWidget(parent),
	d(new TabLogViewsWidgetPrivate()) {

	d->contextMenu = NULL;

	QPushButton* tabNewTabButton=new QPushButton(SmallIcon(QLatin1String( "tab-new" )), QLatin1String( "" ), this);
	connect(tabNewTabButton, SIGNAL(clicked()), this, SLOT(createTab()));

	tabNewTabButton->setToolTip(i18n("Create a new tab"));
	tabNewTabButton->setWhatsThis(i18n("Creates a new tab which can display another log."));

	QPushButton* tabCloseTabButton=new QPushButton(SmallIcon(QLatin1String( "tab-close" )), QLatin1String( "" ), this);
	connect(tabCloseTabButton, SIGNAL(clicked()), this, SLOT(closeTab()));

	tabCloseTabButton->setToolTip(i18n("Close the current tab"));
	tabCloseTabButton->setWhatsThis(i18n("Closes the current tab."));

	setCornerWidget(tabNewTabButton, Qt::TopLeftCorner);
	setCornerWidget(tabCloseTabButton, Qt::TopRightCorner);

	setUsesScrollButtons(true);

	//The context menu is managed manually
  setContextMenuPolicy(Qt::ActionsContextMenu);

  //connect(this, SIGNAL(mouseDoubleClick()), this, SLOT(createTab()));
  //connect(this, SIGNAL(contextMenu(QPoint)), this, SLOT(showContextMenu(QPoint)));
  //connect(this, SIGNAL(contextMenu(QWidget*,QPoint)), this, SLOT(showContextMenu(QWidget*,QPoint)));

	//TODO Use this (need to connect to movedTab(int, int) signal and update the QList
	//setTabReorderingEnabled(true);

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(changeCurrentTab()));
}

TabLogViewsWidget::~TabLogViewsWidget() {
	//TODO Try to do not crash KSystemLog at exitting
	/*
	QList<TabLogManager*> copy = d->tabLogManagers;

	//Delete existing tabs and related tabLogManagers
	foreach(TabLogManager* tabLogManager, copy) {
    logDebug() << "Deleting " << tabLogManager->logManager()->logMode()->name();
		removePage(tabLogManager->logManager()->usedView());

		d->tabLogManagers.removeAll(tabLogManager);
		delete tabLogManager;
    logDebug() << tabLogManager->logManager()->logMode()->name() << " deleted";
	}
	*/

	delete d;
}

void TabLogViewsWidget::newTab(View* view) {
  logDebug() << "Inserting to a new tab the view ";

	//Add a tab at the end of the widget
	insertTab(count(), view, SmallIcon(QLatin1String( NO_MODE_ICON )), i18n("No Log"));

	if (count()>1)
    tabBar()->show();

	else
    tabBar()->hide();

}

void TabLogViewsWidget::changeTab(View* view, const QIcon& icon, const QString& label) {
  logDebug() << "Changing tab " << label;
	int index = indexOf(view);
	setTabIcon(index, icon);
	setTabText(index, label);
}

QList<LogManager*> TabLogViewsWidget::logManagers() {
	QList<LogManager*> logManagers;
	foreach(TabLogManager* tabLogManager, d->tabLogManagers) {
		logManagers.append(tabLogManager->logManager());
	}

	return logManagers;
}

LogManager* TabLogViewsWidget::findRelatedLogManager(View* view) {
	return findRelatedTabLogManager(view)->logManager();
}

TabLogManager* TabLogViewsWidget::findRelatedTabLogManager(View* view) {
	foreach (TabLogManager* tabLogManager, d->tabLogManagers) {
		if (tabLogManager->logManager()->usedView()==view) {
			return tabLogManager;
		}
	}

  logCritical() << "No log manager found";
	return NULL;
}

TabLogManager* TabLogViewsWidget::activeTabLogManager() {
	View* currentView=static_cast<View*> (currentWidget());

	return findRelatedTabLogManager(currentView);
}

LogManager* TabLogViewsWidget::activeLogManager() {
	return activeTabLogManager()->logManager();
}

LogManager* TabLogViewsWidget::createTab() {
  logDebug() << "Creating a new tab";

	return newTabLogManager()->logManager();
}

void TabLogViewsWidget::moveTabLeft() {
  logDebug() << "Duplicate tab to the left";

	TabLogManager* currentTabLogManager=activeTabLogManager();
	int position=indexOf(currentTabLogManager->logManager()->usedView());

	if (position<=0) {
    logCritical() << "Tab Position <= 0 : " << position;
		return;
	}

	d->tabLogManagers.removeAt(position);
	d->tabLogManagers.insert(position-1, currentTabLogManager);

  tabBar()->moveTab(position, position-1);
}

void TabLogViewsWidget::moveTabRight() {
  logDebug() << "Duplicate tab to the right";

	TabLogManager* currentTabLogManager=activeTabLogManager();
	int position=indexOf(currentTabLogManager->logManager()->usedView());

	if (position>=count()-1) {
    logCritical() << "Tab Position >= count()-1 : " << position;
		return;
	}

	d->tabLogManagers.removeAt(position);
	d->tabLogManagers.insert(position+1, currentTabLogManager);

  tabBar()->moveTab(position, position+1);
}

LogManager* TabLogViewsWidget::duplicateTab() {
  logDebug() << "Duplicate current tab";

	TabLogManager* currentManager=activeTabLogManager();

	TabLogManager* tabLogManager=newTabLogManager();

	LogMode* mode=currentManager->logManager()->logMode();

	load(mode, tabLogManager->logManager());

	//Returns the newly created LogManager
	return tabLogManager->logManager();
}

TabLogManager* TabLogViewsWidget::newTabLogManager() {
  logDebug() << "Creating new View...";

	View* view = new View(this);

  logDebug() << "Creating new LogManager...";

	LogManager* logManager=new LogManager(view);

	//Signals from LogManager to Main Class
	connect(logManager, SIGNAL(tabTitleChanged(View*,QIcon,QString)), this, SLOT(changeTab(View*,QIcon,QString)));
	connect(logManager, SIGNAL(logUpdated(View*,int)), this, SLOT(changeTitleAddedLines(View*,int)));

	TabLogManager* tabLogManager = new TabLogManager(logManager);
	d->tabLogManagers.append(tabLogManager);

  logDebug() << "New LogManager created";

	//Finally add the view to the tabs
	newTab(view);

	emit logManagerCreated(logManager);

	setCurrentIndex(count()-1);

	//Set focus to the list
	view->logViewWidget()->setFocus();

	//Returns the newly created TabLogManager
	return tabLogManager;

}

void TabLogViewsWidget::closeTab() {
	if (count()==1) {
    logCritical() << "Cannot close tab, one tab left";
		return;
	}

  TabLogManager* currentTabLogManager = activeTabLogManager();

	d->tabLogManagers.removeAll(currentTabLogManager);

  removeTab(indexOf(currentTabLogManager->logManager()->usedView()));
  if (count() == 1) {
    tabBar()->hide();
	}

	delete currentTabLogManager;
}

void TabLogViewsWidget::load(LogMode* logMode, LogManager* manager) {
  logDebug() << "Loading a new mode : " << logMode->name();

	if (manager==NULL || logMode==NULL) {
    logCritical() << "Error while loading a manager ";
		return;
	}

	//The manager is now using the Log mode passed in parameter
	manager->initialize(logMode);

	//Launch the reading
	manager->reload();
}

void TabLogViewsWidget::reloadCurrent() {
  logDebug() << "Reloading current log manager...";

	LogManager* manager=activeLogManager();

	if (manager!=NULL) {
		manager->reload();
	}

}

void TabLogViewsWidget::reloadAll() {
  logDebug() << "Reloading all tabs...";

	foreach (TabLogManager* tabLogManager, d->tabLogManagers) {
		//Log manager without log mode does not need to be reloaded
		if (tabLogManager->logManager()->logMode() == NULL) {
			continue;
		}

		//Do a simple reload if it is an open log mode
		if (tabLogManager->logManager()->logMode()->id()==QLatin1String( "openLogMode" )) {
			tabLogManager->logManager()->reload();
			continue;
		}

		//Do a full loading of other log modes (needed if log files have been modified)
		load(tabLogManager->logManager()->logMode(), tabLogManager->logManager());

	}
}

void TabLogViewsWidget::changeCurrentTab() {
  logDebug() << "Changing current tab...";

	TabLogManager* tabLogManager=activeTabLogManager();

	//Reinit the new lines count since last selection
	tabLogManager->initNewLinesCount();

	//If the tab displayed the new added line count, rename it to the default log mode name
	changeTab(tabLogManager->logManager()->usedView(), logModeIcon(tabLogManager->logManager()->logMode()), tabLogManager->title());

  logDebug() << "Current tab changed";
}

void TabLogViewsWidget::changeReloadingTab(View* view, bool reloading) {
	TabLogManager* tabLogManager = findRelatedTabLogManager(view);

	if (reloading == true)
		changeTab(tabLogManager->logManager()->usedView(), QIcon::fromTheme( QLatin1String( "view-refresh" )), tabLogManager->title());
	else
		changeTab(tabLogManager->logManager()->usedView(), logModeIcon(tabLogManager->logManager()->logMode()), tabLogManager->title());
}

void TabLogViewsWidget::changeTitleAddedLines(View* view, int addedLinesSinceLastUpdate) {
  logDebug() << "Changing title" << addedLinesSinceLastUpdate << " added lines...";
	LogManager* currentManager=activeLogManager();

	//Only display added line on tab title if this is not an update of the current tab
	if (currentManager->usedView() != view) {
		TabLogManager* tabLogManager = findRelatedTabLogManager(view);
		tabLogManager->addNewLinesCount(addedLinesSinceLastUpdate);

		//Update the tab title
		changeTab(tabLogManager->logManager()->usedView(), logModeIcon(tabLogManager->logManager()->logMode()), tabLogManager->title());
	}
}
void TabLogViewsWidget::expandAllCurrentView() {
	activeLogManager()->usedView()->logViewWidget()->expandAll();
}

void TabLogViewsWidget::collapseAllCurrentView() {
	activeLogManager()->usedView()->logViewWidget()->collapseAll();
}

void TabLogViewsWidget::selectAllCurrentView() {
	activeLogManager()->usedView()->logViewWidget()->selectAll();
}

void TabLogViewsWidget::fileSaveCurrentView() {
	LogViewExport logViewExport(this, activeLogManager()->usedView()->logViewWidget());
	connect(&logViewExport, SIGNAL(statusBarChanged(QString)), this, SIGNAL(statusBarChanged(QString)));
	logViewExport.fileSave();
}

void TabLogViewsWidget::copyToClipboardCurrentView() {
	LogViewExport logViewExport(this, activeLogManager()->usedView()->logViewWidget());
	connect(&logViewExport, SIGNAL(statusBarChanged(QString)), this, SIGNAL(statusBarChanged(QString)));
	logViewExport.copyToClipboard();
}
void TabLogViewsWidget::sendMailCurrentView() {
	LogViewExport logViewExport(this, activeLogManager()->usedView()->logViewWidget());
	connect(&logViewExport, SIGNAL(statusBarChanged(QString)), this, SIGNAL(statusBarChanged(QString)));
	logViewExport.sendMail();
}
void TabLogViewsWidget::printSelectionCurrentView() {
	LogViewExport logViewExport(this, activeLogManager()->usedView()->logViewWidget());
	connect(&logViewExport, SIGNAL(statusBarChanged(QString)), this, SIGNAL(statusBarChanged(QString)));
	logViewExport.printSelection();
}

QIcon TabLogViewsWidget::logModeIcon(LogMode* logMode) {
	if ( logMode == NULL)
		return QIcon::fromTheme(QLatin1String( NO_MODE_ICON ));
	else
		return logMode->icon();

}

void TabLogViewsWidget::prepareContextMenu(bool /*onTab*/) {
	if (d->contextMenu == NULL) {
		d->contextMenu = new QMenu(this);
		d->contextMenu->addActions(actions());
	}

	//TODO Disable some actions, depending of the onTab value
}

void TabLogViewsWidget::showContextMenu(const QPoint& cursorPosition) {
  logDebug() << "Showing context menu at " << cursorPosition;

	prepareContextMenu(false);

  d->contextMenu->popup(cursorPosition);
}

void TabLogViewsWidget::showContextMenu(QWidget* tab, const QPoint& cursorPosition) {
  logDebug() << "Showing context menu at " << cursorPosition << " at " << tab->objectName();

	prepareContextMenu(true);

  d->contextMenu->popup(cursorPosition);
}

