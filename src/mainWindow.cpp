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

#include "mainWindow.h"

//Qt includes

#include <QActionGroup>
#include <QAction>
#include <QList>
#include <QPrinter>

//KDE includes
#include <kmainwindow.h>
#include <kmessagebox.h>
#include <ktabwidget.h>
#include <kactionmenu.h>
#include <kglobal.h>
#include <KLocalizedString>

#include <QIcon>

#include <kservice.h>
#include <kservicetypetrader.h>

#include <ktoggleaction.h>

#include <kfiledialog.h>

#include <kconfig.h>

#include <kurlrequesterdialog.h>

#include <kedittoolbar.h>

#include <kstandardshortcut.h>
#include <kstandardaction.h>
#include <kactioncollection.h>

//Project includes
#include "ksystemlogConfig.h"

#include "statusBar.h"

#include "logViewWidget.h"
#include "tabLogViewsWidget.h"
#include "configurationDialog.h"

#include "loggerDialog.h"

#include "detailDialog.h"

#include "logModeConfiguration.h"
#include "logManager.h"

#include "logMode.h"
#include "logLevel.h"
#include "logFile.h"

#include "logging.h"

#include "view.h"
#include "logViewSearchWidget.h"
#include "logModePluginsLoader.h"

#include "globals.h"
#include "logModeAction.h"

namespace KSystemLog {

class MainWindowPrivate {

public:

	QAction* saveAction;
	QAction* copyAction;

	QAction* reloadAction;

	QAction* sendMailAction;
	QAction* logMessageAction;

	QAction* filterBarAction;

	QAction* selectAllAction;

	QAction* expandAllAction;
	QAction* collapseAllAction;

	QAction* resumePauseAction;
	QAction* detailAction;
	QAction* printAction;

	QAction* findAction;
	QAction* findNextAction;
	QAction* findPreviousAction;

	QAction* tooltipEnabledAction;
	QAction* newLinesDisplayedAction;

	/**
	 * Action groups which stores all Log Mode Actions
	 */
	QActionGroup* logModesActionGroup;


	QPrinter* printer;

	/**
	 * Detail dialog
	 */
	DetailDialog* detailDialog;

	/**
	 * Logged Dialog
	 */
	LoggerDialog* loggedDialog;

	ConfigurationDialog* configurationDialog;

	/**
	 * Tab widget managing different views
	 */
	TabLogViewsWidget* tabs;

	KSystemLog::StatusBar* statusBar;

};

MainWindow::MainWindow() :
	KXmlGuiWindow(0),
	d(new MainWindowPrivate())
	{

	d->printer = NULL;
	d->detailDialog = NULL;
	d->configurationDialog = NULL;
	d->tabs = NULL;
	d->statusBar = NULL;

	logDebug() << "Starting KSystemLog..." << endl;

	//Load log modes plugins
	loadLogModePlugins();

	//Create the GUI from XML configuration
	logDebug() << "Creating Gui..." << endl;
	createGUI();

	//TODO Improve the status bar to add a custom widget which shows an history of latest message, and add a LogLevel for each ones
	//Initialize the Status Bar
	setupStatusBar();

	//Setup the main tab bar
	setupTabLogViews();

	//Setup the Actions
	setupActions();

	// Apply the create the main window and ask the mainwindow to
	// automatically save settings if changed: window size, toolbar
	// position, icon size, etc.  Also to add actions for the statusbar
	// toolbar, and keybindings if necessary.
	logDebug() << "Setup Gui..." << endl;
	setupGUI();

	setupLogActions();

	//Apply the saved mainwindow settings, if any, and ask the main window
	//to automatically save settings if changed: window size, tool bar
	//position, icon size, etc.
	setAutoSaveSettings();

	//TODO Try to find an improvement of this _big_ hack to set its default size, the first time KSystemLog is loaded

	//Show KSystemLog before loading the first log file
	show();

	LogManager* firstLogManager = d->tabs->createTab();

	if (KSystemLogConfig::startupLogMode().isEmpty() == false) {
		d->tabs->load(Globals::instance()->findLogMode(KSystemLogConfig::startupLogMode()), firstLogManager);
	}

	//Set focus to the list
	firstLogManager->usedView()->logViewWidget()->setFocus();
}

void MainWindow::loadLogModePlugins() {
	LogModePluginsLoader pluginsLoader(this);
	pluginsLoader.loadPlugins();
}

void MainWindow::setupTabLogViews() {
	logDebug() << "Creating tab widget..." << endl;

	d->tabs=new TabLogViewsWidget();

	connect(d->tabs, SIGNAL(statusBarChanged(QString)), this, SLOT(changeStatusBar(QString)));
	connect(d->tabs, SIGNAL(logManagerCreated(LogManager*)), this, SLOT(prepareCreatedLogManager(LogManager*)));
	connect(d->tabs, SIGNAL(currentChanged(int)), this, SLOT(changeCurrentTab()));

	setCentralWidget(d->tabs);
}

MainWindow::~MainWindow() {
	delete d->loggedDialog;

	delete d->detailDialog;

	delete d->configurationDialog;

	delete d;
}


void MainWindow::setupStatusBar() {
	d->statusBar = new KSystemLog::StatusBar(this);

	setStatusBar(d->statusBar);
}

void MainWindow::prepareCreatedLogManager(LogManager* manager) {
	logDebug() << "Connecting to actions the new log manager and view..." << endl;

	//Contextual menu Log Manager signals
	QAction* separator;

	manager->usedView()->logViewWidget()->addAction(d->reloadAction);
	manager->usedView()->logViewWidget()->addAction(d->selectAllAction);

	separator = new QAction(this);
	separator->setSeparator(true);
	manager->usedView()->logViewWidget()->addAction(separator);

	manager->usedView()->logViewWidget()->addAction(d->copyAction);
	manager->usedView()->logViewWidget()->addAction(d->saveAction);

	separator = new QAction(this);
	separator->setSeparator(true);
	manager->usedView()->logViewWidget()->addAction(separator);

	manager->usedView()->logViewWidget()->addAction(d->tooltipEnabledAction);
	manager->usedView()->logViewWidget()->addAction(d->newLinesDisplayedAction);

	separator = new QAction(this);
	separator->setSeparator(true);
	manager->usedView()->logViewWidget()->addAction(separator);

	manager->usedView()->logViewWidget()->addAction(d->detailAction);

	//Log Manager and View signals
	connect(manager, SIGNAL(windowTitleChanged(QString)), this, SLOT(changeWindowTitle(QString)));
	connect(manager, SIGNAL(statusBarChanged(QString)), this, SLOT(changeStatusBar(QString)));
	connect(manager, SIGNAL(logUpdated(View*,int)), this, SLOT(updateStatusBar()));
	connect(manager, SIGNAL(reloaded()), this, SLOT(changeCurrentTab()));

	connect(manager->usedView(), SIGNAL(searchFilterChanged()), this, SLOT(updateStatusBar()));
	connect(manager->usedView()->logViewWidget(), SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(showDetailsDialog()));
	connect(manager->usedView()->logViewWidget(), SIGNAL(itemSelectionChanged()), this, SLOT(updateSelection()));
	connect(manager->usedView()->logViewWidget()->model(), SIGNAL(processingMultipleInsertions(bool)), this, SLOT(updateReloading()));

	//Correctly initialize the Status Bar
	updateStatusBar();

}

void MainWindow::updateDetailDialog() {
	LogManager* currentManager=d->tabs->activeLogManager();
	if (d->detailDialog!=NULL) {
		d->detailDialog->selectionChanged(currentManager->usedView()->logViewWidget());
	}
}

void MainWindow::updateSelection() {
	//logDebug() << "Updating selection..." << endl;

	LogManager* currentLogManager = d->tabs->activeLogManager();

	updateDetailDialog();

	bool selection=currentLogManager->usedView()->logViewWidget()->hasItemsSelected();

	d->copyAction->setEnabled(selection);
	d->saveAction->setEnabled(selection);
	d->detailAction->setEnabled(selection);
	d->sendMailAction->setEnabled(selection);
	d->printAction->setEnabled(selection);

}

void MainWindow::updateReloading() {
	View* currentView = d->tabs->activeLogManager()->usedView();

	bool enabled = ! currentView->logViewWidget()->model()->isProcessingMultipleInsertions();

	d->reloadAction->setEnabled(enabled);
	d->resumePauseAction->setEnabled(enabled);
	d->findAction->setEnabled(enabled);
	d->findNextAction->setEnabled(enabled);
	d->findPreviousAction->setEnabled(enabled);

	//Enables/Disables all Log Mode actions
	d->logModesActionGroup->setEnabled(enabled);

	d->tabs->changeReloadingTab(currentView, !enabled);

	//Enables/Disables all Log Mode menus (useful for multiple actions menus)
	foreach(LogModeAction* logModeAction, Globals::instance()->logModeActions()) {
		logModeAction->actionMenu()->setEnabled(enabled);
	}

}

void MainWindow::closeEvent(QCloseEvent *event) {
	logDebug() << "Saving configuration before exit..." << endl;
	//Write the config to the file
	KSystemLogConfig::self()->save();
	KXmlGuiWindow::closeEvent(event);
}

TabLogViewsWidget* MainWindow::tabs() {
	return d->tabs;
}

void MainWindow::showDetailsDialog() {
	//Create the Detail dialog if it was not created
	if (d->detailDialog==NULL) {
		d->detailDialog=new DetailDialog(this);
		updateDetailDialog();
	}

	d->detailDialog->show();

}

void MainWindow::toggleItemTooltip(bool enabled) {
	KSystemLogConfig::setTooltipEnabled(enabled);

	foreach (LogManager* manager, d->tabs->logManagers()) {
		manager->usedView()->logViewWidget()->toggleToolTip(enabled);
	}
}

void MainWindow::toggleNewLinesDisplaying(bool displayed) {
	KSystemLogConfig::setNewLinesDisplayed(displayed);
}

void MainWindow::updateStatusBar() {
	logDebug() << "Updating status bar..." << endl;

	LogManager* currentManager=d->tabs->activeLogManager();

	int itemCount = currentManager->usedView()->logViewWidget()->itemCount();
	int notHiddenItemCount = currentManager->usedView()->logViewWidget()->notHiddenItemCount();

	if (itemCount == notHiddenItemCount) {
		d->statusBar->changeLineCountMessage(i18ncp("Total displayed lines", "1 line.", "%1 lines.", currentManager->usedView()->logViewWidget()->itemCount()));
	}
	else {
		d->statusBar->changeLineCountMessage(i18ncp("Line not hidden by search / Total displayed lines", "1 line / %2 total.", "%1 lines / %2 total.", notHiddenItemCount, itemCount));
	}

	d->statusBar->changeLastModification(currentManager->lastUpdate());

}

void MainWindow::toggleResumePauseParsing(bool paused) {
	logDebug() << "Pausing parsing : " << paused << endl;

	LogManager* currentLogManager = d->tabs->activeLogManager();
	if (currentLogManager != NULL) {
		currentLogManager->setParsingPaused(paused);
	}

	logDebug() << "Parsing paused : " << paused << endl;
}

void MainWindow::changeResumePauseAction(bool paused) {

	if (paused == true) {
		d->resumePauseAction->setText(i18n("Resu&me"));
		d->resumePauseAction->setIcon(QIcon::fromTheme( QLatin1String( "media-playback-start" )));
		d->resumePauseAction->setShortcut(Qt::CTRL + Qt::Key_M);
		d->resumePauseAction->setToolTip(i18n("Resume the watching of the current log"));
		d->resumePauseAction->setWhatsThis(i18n("Resumes the watching of the current log. This action is only available when the user has already paused the reading."));
		d->resumePauseAction->setChecked(true);
	}
	else {
		d->resumePauseAction->setText(i18n("S&top"));
		d->resumePauseAction->setIcon(QIcon::fromTheme( QLatin1String( "media-playback-stop" )));
		d->resumePauseAction->setShortcut(Qt::CTRL + Qt::Key_P);
		d->resumePauseAction->setToolTip(i18n("Pause the watching of the current log"));
		d->resumePauseAction->setWhatsThis(i18n("Pauses the watching of the current log. This action is particularly useful when the system is writing too many lines to log files, causing KSystemLog to reload too frequently."));
		d->resumePauseAction->setChecked(false);
	}

	//Be sur that the button will always have a good size
	foreach (QWidget* widget, d->resumePauseAction->associatedWidgets()) {
		if (widget->sizeHint().width() > widget->size().width()) {
			widget->setMinimumSize(widget->sizeHint());
		}
	}
}

void MainWindow::fileOpen() {
	//Launch the actualizing
	d->tabs->load(Globals::instance()->findLogMode(QLatin1String( "openLogMode" )), d->tabs->activeLogManager());
}

void MainWindow::showConfigurationDialog() {
	logDebug() << "Showing configuration dialog..." << endl;

	if (d->configurationDialog == NULL) {
		d->configurationDialog = new ConfigurationDialog(this);
		connect(d->configurationDialog, SIGNAL(configurationSaved()), d->tabs, SLOT(reloadAll()));
	}

	d->configurationDialog->showConfiguration();
}

void MainWindow::showLogMessageDialog() {
	logDebug() << "Launching the Send message dialog box..." << endl;

	if (d->loggedDialog == NULL) {
		d->loggedDialog = new LoggerDialog(this);
	}

	d->loggedDialog->initialize();
	d->loggedDialog->exec();

}

void MainWindow::changeStatusBar(const QString& text) {
	d->statusBar->changeMessage(text);
}

void MainWindow::changeWindowTitle(const QString& text) {
	//Display this text on the caption
	setCaption(text);
}


void MainWindow::changeCurrentTab() {
	logDebug() << "Tab has changed" << endl;

	LogManager* currentManager=d->tabs->activeLogManager();

	//If the tab changed, the selection changes too
	updateSelection();

	//Update the status bar
	updateStatusBar();

	//Updating the current reloading status
	updateReloading();

	bool enabledReloading = ! currentManager->usedView()->logViewWidget()->model()->isProcessingMultipleInsertions();

	bool enabledAction;
	//Change the title of the window
	if (currentManager->logMode() == NULL) {
		changeWindowTitle(i18nc("Newly created tab", "Empty Log"));
		enabledAction = false;
	}
	else {
		changeWindowTitle(currentManager->logMode()->name());
		enabledAction = true;
	}

	if (enabledReloading==true && enabledAction==true) {
		d->reloadAction->setEnabled(true);
		d->resumePauseAction->setEnabled(true);
	}
	else {
		d->reloadAction->setEnabled(false);
		d->resumePauseAction->setEnabled(false);
	}


	//Update Resume/Pause action state
	if (currentManager->isParsingPaused())
		changeResumePauseAction(true);
	else
		changeResumePauseAction(false);

	//Updating Detail dialog
	updateDetailDialog();

	logDebug() << "Tab changing done" << endl;
}

/**
 * TODO Implements the session restoring
 *
 * The 'config' object points to the session managed
 * config file.  anything you write here will be available
 * later when this app is restored
 */
void MainWindow::saveProperties(KConfigGroup & /*configuration*/) {
	logDebug() << "Saving properties..." << endl;
}

/**
 * TODO Implements the session restoring
 *
 * The 'config' object points to the session managed
 * config file.  this function is automatically called whenever
 * the app is being restored.  read in here whatever you wrote
 * in 'saveProperties'
 */
void MainWindow::readProperties( const KConfigGroup& /*configuration*/) {
	logDebug() << "Reading properties..." << endl;
}

void MainWindow::toggleFilterBar() {
	logDebug() << "Toggling filter bar..." << d->filterBarAction->isChecked() << endl;

	foreach (LogManager* manager, d->tabs->logManagers()) {
		manager->usedView()->toggleLogViewFilter(d->filterBarAction->isChecked());
	}

	KSystemLogConfig::setToggleFilterBar(d->filterBarAction->isChecked());
}

void MainWindow::findNext() {
	showSearchBar();
	d->tabs->activeLogManager()->usedView()->logViewSearch()->findNext();
}

void MainWindow::findPrevious() {
	showSearchBar();
	d->tabs->activeLogManager()->usedView()->logViewSearch()->findPrevious();
}

void MainWindow::showSearchBar() {
	logDebug() << "Showing search bar..." << endl;

	LogManager* activeLogManager = d->tabs->activeLogManager();

	foreach (LogManager* manager, d->tabs->logManagers()) {
		if (manager != activeLogManager) {
			manager->usedView()->toggleLogViewSearch(true);
		}
	}

	//Be sure to display the view search of the active LogManager at last, and focus to it
	d->tabs->activeLogManager()->usedView()->toggleLogViewSearch(true);

}

void MainWindow::setupActions() {
	logDebug() << "Creating actions..." << endl;

	QAction* fileOpenAction = actionCollection()->addAction(KStandardAction::Open, this, SLOT(fileOpen()));
	fileOpenAction->setToolTip(i18n("Open a file in KSystemLog"));
	fileOpenAction->setWhatsThis(i18n("Opens a file in KSystemLog and displays its content in the current tab."));

	d->printAction = actionCollection()->addAction(KStandardAction::Print);
	d->printAction->setText(i18n("&Print Selection..."));
	d->printAction->setToolTip(i18n("Print the selection"));
	d->printAction->setWhatsThis(i18n("Prints the selection. Simply select the important lines and click on this menu entry to print the selection."));
	d->printAction->setEnabled(false);

	d->saveAction = actionCollection()->addAction(KStandardAction::SaveAs);
	//TODO Retrieve the system's shortcut of the save action (and not Save as...)
	d->saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
	d->saveAction->setToolTip(i18n("Save the selection to a file"));
	d->saveAction->setWhatsThis(i18n("Saves the selection to a file. This action is useful if you want to create an attachment or a backup of a particular log."));
	d->saveAction->setEnabled(false);

  QAction* fileQuitAction = actionCollection()->addAction(KStandardAction::Quit, qApp, SLOT(quit()));
	fileQuitAction->setToolTip(i18n("Quit KSystemLog"));
	fileQuitAction->setWhatsThis(i18n("Quits KSystemLog."));

	d->copyAction = actionCollection()->addAction(KStandardAction::Copy);
	d->copyAction->setToolTip(i18n("Copy the selection to the clipboard"));
	d->copyAction->setWhatsThis(i18n("Copies the selection to the clipboard. This action is useful if you want to paste the selection in a chat or an email."));
	d->copyAction->setEnabled(false);

	d->expandAllAction = actionCollection()->addAction( QLatin1String( "expand_all" ));
	d->expandAllAction->setText(i18n("Ex&pand All"));
	d->expandAllAction->setShortcut(Qt::CTRL + Qt::Key_X);
	d->expandAllAction->setToolTip(i18n("Expand all categories"));
	d->expandAllAction->setWhatsThis(i18n("This action opens all main categories. This is enabled only if an option has been selected in the <b>Group By</b> menu."));
	d->expandAllAction->setEnabled(false);

	d->collapseAllAction = actionCollection()->addAction( QLatin1String( "collapse_all" ));
	d->collapseAllAction->setText(i18n("Col&lapse All"));
	d->collapseAllAction->setShortcut(Qt::CTRL + Qt::Key_L);
	d->collapseAllAction->setToolTip(i18n("Collapse all categories"));
	d->collapseAllAction->setWhatsThis(i18n("This action closes all main categories. This is enabled only if an option has been selected in the <b>Group By</b> menu."));
	d->collapseAllAction->setEnabled(false);

	d->sendMailAction = actionCollection()->addAction( QLatin1String( "send_mail" ));
	d->sendMailAction->setText(i18n("&Email Selection..."));
	d->sendMailAction->setIcon(QIcon::fromTheme( QLatin1String( "mail-message-new" )));
	d->sendMailAction->setShortcut(Qt::CTRL + Qt::Key_M);
	d->sendMailAction->setToolTip(i18n("Send the selection by mail"));
	d->sendMailAction->setWhatsThis(i18n("Sends the selection by mail. Simply select the important lines and click on this menu entry to send the selection to a friend or a mailing list."));
	d->sendMailAction->setEnabled(false);

	d->logMessageAction = actionCollection()->addAction( QLatin1String( "log_message" ), this, SLOT(showLogMessageDialog()));
	d->logMessageAction->setText(i18n("&Add Log Entry..."));
	d->logMessageAction->setIcon(QIcon::fromTheme( QLatin1String( "document-new" )));
	d->logMessageAction->setShortcut(Qt::CTRL + Qt::Key_L);
	d->logMessageAction->setToolTip(i18n("Add a log entry to the log system"));
	d->logMessageAction->setWhatsThis(i18n("This action will open a dialog which lets you send a message to the log system."));

	d->selectAllAction = actionCollection()->addAction(KStandardAction::SelectAll);
	d->selectAllAction->setToolTip(i18n("Select all lines of the current log"));
	d->selectAllAction->setWhatsThis(i18n("Selects all lines of the current log. This action is useful if you want, for example, to save all the content of the current log in a file."));

	d->findAction = actionCollection()->addAction(KStandardAction::Find, this, SLOT(showSearchBar()));
	d->findNextAction = actionCollection()->addAction(KStandardAction::FindNext, this, SLOT(findNext()));
	d->findPreviousAction = actionCollection()->addAction(KStandardAction::FindPrev, this, SLOT(findPrevious()));


	actionCollection()->addAction(KStandardAction::Preferences, this, SLOT(showConfigurationDialog()));

	//TODO Find a solution to display at the right place this action (see Akregator interface)
	d->filterBarAction = actionCollection()->addAction( QLatin1String( "show_quick_filter" ), this, SLOT(toggleFilterBar()));
	d->filterBarAction->setText(i18n("Show &Filter Bar"));
	d->filterBarAction->setEnabled(true);
	d->filterBarAction->setCheckable(true);
	d->filterBarAction->setChecked(KSystemLogConfig::toggleFilterBar());

	QAction* newTabAction = actionCollection()->addAction( QLatin1String( "new_tab" ), d->tabs, SLOT(createTab()));
	newTabAction->setText(i18n("&New Tab"));
	newTabAction->setIcon(QIcon::fromTheme( QLatin1String( "tab-new" )));
	newTabAction->setShortcut(Qt::CTRL + Qt::Key_T);
	newTabAction->setToolTip(i18n("Create a new tab"));
	newTabAction->setWhatsThis(i18n("Creates a new tab which can display another log."));
	d->tabs->addAction(newTabAction);

	QAction* closeTabAction = actionCollection()->addAction( QLatin1String( "close_tab" ), d->tabs, SLOT(closeTab()));
	closeTabAction->setText(i18n("&Close Tab"));
	closeTabAction->setIcon(QIcon::fromTheme( QLatin1String( "tab-close" )));
	closeTabAction->setShortcut(Qt::CTRL+Qt::Key_W);
	closeTabAction->setToolTip(i18n("Close the current tab"));
	closeTabAction->setWhatsThis(i18n("Closes the current tab."));
	d->tabs->addAction(closeTabAction);

	QAction* duplicateTabAction = actionCollection()->addAction( QLatin1String( "duplicate_tab" ), d->tabs, SLOT(duplicateTab()));
	duplicateTabAction->setText(i18n("&Duplicate Tab"));
	duplicateTabAction->setIcon(QIcon::fromTheme( QLatin1String( "tab-duplicate" )));
	duplicateTabAction->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_N);
	duplicateTabAction->setToolTip(i18n("Duplicate the current tab"));
	duplicateTabAction->setWhatsThis(i18n("Duplicates the current tab."));
	d->tabs->addAction(duplicateTabAction);

	QAction* separatorAction = new QAction(this);
	separatorAction->setSeparator(true);
	d->tabs->addAction(separatorAction);

	QAction* moveTabLeftAction = actionCollection()->addAction( QLatin1String( "move_tab_left" ), d->tabs, SLOT(moveTabLeft()));
	moveTabLeftAction->setText(i18n("Move Tab &Left"));
	moveTabLeftAction->setIcon(QIcon::fromTheme( QLatin1String( "arrow-left" )));
	moveTabLeftAction->setShortcut(Qt::SHIFT+Qt::CTRL+Qt::Key_Left);
	moveTabLeftAction->setToolTip(i18n("Move the current tab to the left"));
	moveTabLeftAction->setWhatsThis(i18n("Moves the current tab to the left."));
	d->tabs->addAction(moveTabLeftAction);

	QAction* moveTabRightAction = actionCollection()->addAction( QLatin1String( "move_tab_right" ), d->tabs, SLOT(moveTabRight()));
	moveTabRightAction->setText(i18n("Move Tab &Right"));
	moveTabRightAction->setIcon(QIcon::fromTheme( QLatin1String( "arrow-right" )));
	moveTabRightAction->setShortcut(Qt::SHIFT+Qt::CTRL+Qt::Key_Right);
	moveTabRightAction->setToolTip(i18n("Move the current tab to the right"));
	moveTabRightAction->setWhatsThis(i18n("Moves the current tab to the right."));
	d->tabs->addAction(moveTabRightAction);

	d->reloadAction = actionCollection()->addAction( QLatin1String( "reload" ), d->tabs, SLOT(reloadCurrent()));
	d->reloadAction->setText(i18n("&Reload"));
	d->reloadAction->setIcon(QIcon::fromTheme( QLatin1String( "view-refresh" )));
	d->reloadAction->setShortcut(Qt::Key_F5);
	d->reloadAction->setToolTip(i18n("Reload the current log"));
	d->reloadAction->setWhatsThis(i18n("Reloads the current log, if you want to be sure that the view is correctly updated."));

	d->resumePauseAction = actionCollection()->addAction( QLatin1String( "resume_pause_parsing" ));
	d->resumePauseAction->setCheckable(true);
	connect(d->resumePauseAction, SIGNAL(toggled(bool)), this, SLOT(changeResumePauseAction(bool)));
	connect(d->resumePauseAction, SIGNAL(toggled(bool)), this, SLOT(toggleResumePauseParsing(bool)));
	changeResumePauseAction(false);

	d->detailAction = actionCollection()->addAction( QLatin1String( "details" ), this, SLOT(showDetailsDialog()));
	d->detailAction->setText(i18n("&Details"));
	d->detailAction->setIcon(QIcon::fromTheme( QLatin1String( "document-preview" )));
	d->detailAction->setShortcut(Qt::ALT + Qt::Key_Return);
	d->detailAction->setToolTip(i18n("Display details on the selected line"));
	d->detailAction->setWhatsThis(i18n("Displays a dialog box which contains details on the selected line. You are able to navigate through the logs from this dialog box with the <b>Previous</b> and <b>Next</b> buttons."));
	d->detailAction->setEnabled(false);

	d->tooltipEnabledAction = actionCollection()->addAction( QLatin1String( "tooltipEnabled" ));
	d->tooltipEnabledAction->setText(i18n("&Enable Detailed Tooltips"));
	d->tooltipEnabledAction->setToolTip(i18n("Disable/Enable the tooltip on the current view"));
	d->tooltipEnabledAction->setWhatsThis(i18n("Disables/Enables the tooltip displayed when the cursor hovers a log line."));
	d->tooltipEnabledAction->setCheckable(true);
	d->tooltipEnabledAction->setChecked(KSystemLogConfig::tooltipEnabled());
	connect(d->tooltipEnabledAction, SIGNAL(toggled(bool)), this, SLOT(toggleItemTooltip(bool)));

	d->newLinesDisplayedAction = actionCollection()->addAction( QLatin1String( "newLinesDisplayed" ));
	d->newLinesDisplayedAction->setText(i18n("&Scroll to New Lines"));
	d->newLinesDisplayedAction->setToolTip(i18n("Scrolls or not to the new lines when the log changes"));
	d->newLinesDisplayedAction->setWhatsThis(i18n("Scrolls or not to the new lines when the log changes. Check this option if you do not want the application to scroll automatically at the bottom of the log each time it is refreshed."));
	d->newLinesDisplayedAction->setCheckable(true);
	d->newLinesDisplayedAction->setChecked(KSystemLogConfig::newLinesDisplayed());
	connect(d->newLinesDisplayedAction, SIGNAL(toggled(bool)), this, SLOT(toggleNewLinesDisplaying(bool)));

	//Toolbar and Menu signals
	connect(d->expandAllAction, SIGNAL(triggered()), d->tabs, SLOT(expandAllCurrentView()));
	connect(d->collapseAllAction, SIGNAL(triggered()), d->tabs, SLOT(collapseAllCurrentView()));
	connect(d->saveAction, SIGNAL(triggered()), d->tabs, SLOT(fileSaveCurrentView()));
	connect(d->copyAction, SIGNAL(triggered()), d->tabs, SLOT(copyToClipboardCurrentView()));
	connect(d->sendMailAction, SIGNAL(triggered()), d->tabs, SLOT(sendMailCurrentView()));
	connect(d->printAction, SIGNAL(triggered()), d->tabs, SLOT(printSelectionCurrentView()));
	connect(d->selectAllAction, SIGNAL(triggered()), d->tabs, SLOT(selectAllCurrentView()));

}

void MainWindow::selectLogModeAction(QAction* action) {
	QString selectedModeId = action->data().toString();

	LogMode* currentMode = NULL;
	foreach(LogMode* logMode, Globals::instance()->logModes()) {
		if (logMode->id() == selectedModeId) {
			currentMode = logMode;
			break;
		}
	}

	if (currentMode==NULL) {
		logError() << "The selected mode does not exist" << endl;
		return;
	}

	logDebug() << "Selecting " << currentMode->name() << " (" << currentMode->id() << ")" << endl;

	/*
	//If the user uses the middle button OR left button + shift OR left button + control : = it opens the log in a new tab
	if (state==Qt::MidButton || (state==Qt::ControlModifier+Qt::LeftButton) || (state==Qt::ShiftModifier+Qt::LeftButton))
		createTab();
	*/

	d->tabs->load(currentMode, d->tabs->activeLogManager());
}

void MainWindow::setupLogActions() {
	QList<QAction*> menuLogModeActions;
	QList<QAction*> toolBarLogModeActions;

	KActionMenu* servicesAction = new KActionMenu(QIcon::fromTheme( QLatin1String( "preferences-system-session-services") ), i18n("Services"), this);
	KActionMenu* othersAction = new KActionMenu(QIcon::fromTheme( QLatin1String( "preferences-other")), i18n("Others"), this);

	d->logModesActionGroup=new QActionGroup(actionCollection());

	connect(d->logModesActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(selectLogModeAction(QAction*)));

	foreach(LogModeAction* logModeAction, Globals::instance()->logModeActions()) {
		foreach(QAction* action, logModeAction->innerActions()) {
			d->logModesActionGroup->addAction(action);
		}

		if (logModeAction->category() == LogModeAction::RootCategory)
			menuLogModeActions.append( logModeAction->actionMenu() );
		else if (logModeAction->category() == LogModeAction::ServicesCategory)
			servicesAction->addAction( logModeAction->actionMenu() );
		else if (logModeAction->category() == LogModeAction::OthersCategory)
			othersAction->addAction( logModeAction->actionMenu() );

		if (logModeAction->isInToolBar() == true) {
			toolBarLogModeActions.append( logModeAction->actionMenu() );
		}
	}

	menuLogModeActions.append( servicesAction );
	menuLogModeActions.append( othersAction );

	//Menu dynamic action list
	unplugActionList(QLatin1String( "log_mode_list" ));
	plugActionList(QLatin1String( "log_mode_list" ), menuLogModeActions);

	//ToolBar dynamic action list
	unplugActionList(QLatin1String( "tool_bar_log_mode_list" ));
	plugActionList(QLatin1String( "tool_bar_log_mode_list" ), toolBarLogModeActions);

}

}
