/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainWindow.h"

// Qt includes
#include <QAction>
#include <QIcon>
#include <QList>

// KDE includes
#include <KActionCollection>
#include <KActionMenu>
#include <KLocalizedString>
#include <KStandardAction>
#include <KStandardShortcut>

// Project includes
#include "ksystemlogConfig.h"

#include "statusBar.h"

#include "configurationDialog.h"
#include "logViewWidget.h"
#include "tabLogViewsWidget.h"

#include "loggerDialog.h"

#include "detailDialog.h"

#include "logManager.h"
#include "logModeConfiguration.h"

#include "logFile.h"
#include "logLevel.h"
#include "logMode.h"

#include "ksystemlog_debug.h"

#include "logModePluginsLoader.h"
#include "logViewSearchWidget.h"
#include "view.h"

#include "globals.h"

namespace KSystemLog
{
MainWindow::MainWindow()
    : KXmlGuiWindow(nullptr)
{
    qCDebug(KSYSTEMLOG) << "Starting KSystemLog...";

    // Load log modes plugins
    loadLogModePlugins();

    // Create the GUI from XML configuration
    qCDebug(KSYSTEMLOG) << "Creating Gui...";
    createGUI();

    // TODO Improve the status bar to add a custom widget which shows an history of latest message, and add a
    // LogLevel for each ones
    // Initialize the Status Bar
    setupStatusBar();

    // Setup the main tab bar
    setupTabLogViews();

    // Setup the Actions
    setupActions();

    // Setup toolbar log actions, needs to be called before setupGUI()
    setupLogActions();

    // Apply the create the main window and ask the mainwindow to
    // automatically save settings if changed: window size, toolbar
    // position, icon size, etc.  Also to add actions for the statusbar
    // toolbar, and keybindings if necessary.
    qCDebug(KSYSTEMLOG) << "Setup Gui...";
    setupGUI();

    // Setup Logs menu, needs to be called after setupGUI()
    setupLogModeMenu();

    // Apply the saved mainwindow settings, if any, and ask the main window
    // to automatically save settings if changed: window size, tool bar
    // position, icon size, etc.
    setAutoSaveSettings();

    mConfigurationDialog = new ConfigurationDialog(this);
    connect(mConfigurationDialog, &ConfigurationDialog::configurationSaved, mTabs, &TabLogViewsWidget::reloadAll);

    // Show KSystemLog before loading the first log file
    show();

    LogManager *firstLogManager = mTabs->createTab();

    // Load selected mode only if its log files exist.
    const QString &startupLogMode = KSystemLogConfig::startupLogMode();
    if (!startupLogMode.isEmpty()) {
        LogMode *mode = Globals::instance().findLogMode(startupLogMode);
        if (mode) {
            if (mode->filesExist()) {
                mTabs->load(mode, firstLogManager);
            } else {
                qCWarning(KSYSTEMLOG) << mode->name() << "is selected by default, but log files do not exist.";
            }
        }
    }

    // Set focus to the list
    firstLogManager->usedView()->logViewWidget()->setFocus();

    const auto logModes = Globals::instance().logModes();
    for (LogMode *logMode : logModes) {
        connect(logMode, &LogMode::menuChanged, this, &MainWindow::recreateActions);
    }
}

void MainWindow::loadLogModePlugins()
{
    LogModePluginsLoader pluginsLoader(this);
    pluginsLoader.loadPlugins();
}

void MainWindow::setupTabLogViews()
{
    qCDebug(KSYSTEMLOG) << "Creating tab widget...";

    mTabs = new TabLogViewsWidget(this);

    connect(mTabs, &TabLogViewsWidget::statusBarChanged, this, &MainWindow::changeStatusBar);
    connect(mTabs, &TabLogViewsWidget::logManagerCreated, this, &MainWindow::prepareCreatedLogManager);
    connect(mTabs, &QTabWidget::currentChanged, this, &MainWindow::changeCurrentTab);

    setCentralWidget(mTabs);
}

MainWindow::~MainWindow()
{
    delete mLoggedDialog;

    delete mDetailDialog;

    delete mConfigurationDialog;
}

void MainWindow::setupStatusBar()
{
    mStatusBar = new KSystemLog::StatusBar(this);

    setStatusBar(mStatusBar);
}

void MainWindow::prepareCreatedLogManager(LogManager *manager)
{
    qCDebug(KSYSTEMLOG) << "Connecting to actions the new log manager and view...";

    // Contextual menu Log Manager signals

    manager->usedView()->logViewWidget()->addAction(mReloadAction);
    manager->usedView()->logViewWidget()->addAction(mSelectAllAction);

    auto separator = new QAction(this);
    separator->setSeparator(true);
    manager->usedView()->logViewWidget()->addAction(separator);

    manager->usedView()->logViewWidget()->addAction(mCopyAction);
    manager->usedView()->logViewWidget()->addAction(mSaveAction);
    manager->usedView()->logViewWidget()->addAction(mSendMailAction);

    separator = new QAction(this);
    separator->setSeparator(true);
    manager->usedView()->logViewWidget()->addAction(separator);

    manager->usedView()->logViewWidget()->addAction(mTooltipEnabledAction);
    manager->usedView()->logViewWidget()->addAction(mNewLinesDisplayedAction);

    separator = new QAction(this);
    separator->setSeparator(true);
    manager->usedView()->logViewWidget()->addAction(separator);

    manager->usedView()->logViewWidget()->addAction(mDetailAction);

    // Log Manager and View signals
    connect(manager, &LogManager::windowTitleChanged, this, &MainWindow::changeWindowTitle);
    connect(manager, &LogManager::statusBarChanged, this, &MainWindow::changeStatusBar);
    connect(manager, &LogManager::logUpdated, this, &MainWindow::updateStatusBar);
    connect(manager, &LogManager::reloaded, this, &MainWindow::changeCurrentTab);

    connect(manager->usedView(), &View::searchFilterChanged, this, &MainWindow::updateStatusBar);
    connect(manager->usedView()->logViewWidget(), &QTreeWidget::itemDoubleClicked, this, &MainWindow::showDetailsDialog);
    connect(manager->usedView()->logViewWidget(), &QTreeWidget::itemSelectionChanged, this, &MainWindow::updateSelection);
    connect(manager->usedView()->logViewWidget()->model(), &LogViewModel::processingMultipleInsertions, this, &MainWindow::updateReloading);

    // Correctly initialize the Status Bar
    updateStatusBar();
}

void MainWindow::updateDetailDialog()
{
    LogManager *currentManager = mTabs->activeLogManager();
    if (mDetailDialog) {
        mDetailDialog->selectionChanged(currentManager->usedView()->logViewWidget());
    }
}

void MainWindow::updateSelection()
{
    // qCDebug(KSYSTEMLOG) << "Updating selection...";

    LogManager *currentLogManager = mTabs->activeLogManager();

    updateDetailDialog();

    bool const selection = currentLogManager->usedView()->logViewWidget()->hasItemsSelected();

    mCopyAction->setEnabled(selection);
    mSaveAction->setEnabled(selection);
    mDetailAction->setEnabled(selection);
    mSendMailAction->setEnabled(selection);
    mPrintAction->setEnabled(selection);
    mPrintPreviewAction->setEnabled(selection);
}

void MainWindow::updateReloading()
{
    View *currentView = mTabs->activeLogManager()->usedView();

    const bool enabled = !currentView->logViewWidget()->model()->isProcessingMultipleInsertions();

    mReloadAction->setEnabled(enabled);
    mResumePauseAction->setEnabled(enabled);
    mFindAction->setEnabled(enabled);
    mFindNextAction->setEnabled(enabled);
    mFindPreviousAction->setEnabled(enabled);

    // Enables/Disables all Log Mode actions
    // logModesActionGroup->setEnabled(enabled);

    mTabs->changeReloadingTab(currentView, !enabled);

    // Enables/Disables all Log Mode menus (useful for multiple actions menus)
    const auto logModeActions{Globals::instance().logModeActions()};
    for (LogModeAction *logModeAction : logModeActions) {
        logModeAction->actionMenu()->setEnabled(enabled);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    disconnect(mTabs, &QTabWidget::currentChanged, this, &MainWindow::changeCurrentTab);

    LogManager *currentLogManager = mTabs->activeLogManager();
    if (currentLogManager) {
        currentLogManager->stopWatching();
    }
    qCDebug(KSYSTEMLOG) << "Saving configuration before exit...";
    qCDebug(KSYSTEMLOG) << "Saving configuration before exit...";
    // Write the config to the file
    KSystemLogConfig::self()->save();
    KXmlGuiWindow::closeEvent(event);
}

TabLogViewsWidget *MainWindow::tabs()
{
    return mTabs;
}

void MainWindow::showDetailsDialog()
{
    // Create the Detail dialog if it was not created
    if (!mDetailDialog) {
        mDetailDialog = new DetailDialog(this);
        updateDetailDialog();
    }

    mDetailDialog->show();
}

void MainWindow::toggleItemTooltip(bool enabled)
{
    KSystemLogConfig::setTooltipEnabled(enabled);

    const auto logManagers{mTabs->logManagers()};
    for (LogManager *manager : logManagers) {
        manager->usedView()->logViewWidget()->toggleToolTip(enabled);
    }
}

void MainWindow::toggleNewLinesDisplaying(bool displayed)
{
    KSystemLogConfig::setNewLinesDisplayed(displayed);
}

void MainWindow::updateStatusBar()
{
    qCDebug(KSYSTEMLOG) << "Updating status bar...";

    LogManager *currentManager = mTabs->activeLogManager();

    int const itemCount = currentManager->usedView()->logViewWidget()->itemCount();
    int const notHiddenItemCount = currentManager->usedView()->logViewWidget()->notHiddenItemCount();

    if (itemCount == notHiddenItemCount) {
        mStatusBar->changeLineCountMessage(i18ncp("Total displayed lines", "1 line.", "%1 lines.", currentManager->usedView()->logViewWidget()->itemCount()));
    } else {
        mStatusBar->changeLineCountMessage(
            i18ncp("Line not hidden by search / Total displayed lines", "1 line / %2 total.", "%1 lines / %2 total.", notHiddenItemCount, itemCount));
    }

    mStatusBar->changeLastModification(currentManager->lastUpdate());
}

void MainWindow::toggleResumePauseParsing(bool paused)
{
    qCDebug(KSYSTEMLOG) << "Pausing parsing : " << paused;

    LogManager *currentLogManager = mTabs->activeLogManager();
    if (currentLogManager) {
        currentLogManager->setParsingPaused(paused);
    }

    qCDebug(KSYSTEMLOG) << "Parsing paused : " << paused;
}

void MainWindow::changeResumePauseAction(bool paused)
{
    if (paused) {
        mResumePauseAction->setText(i18n("Resu&me"));
        mResumePauseAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
        mResumePauseAction->setToolTip(i18n("Resume the watching of the current log"));
        mResumePauseAction->setWhatsThis(
            i18n("Resumes the watching of the current log. This action is only available when the user has "
                 "already paused the reading."));
        mResumePauseAction->setChecked(true);
        actionCollection()->setDefaultShortcut(mResumePauseAction, Qt::CTRL | Qt::Key_M);
    } else {
        mResumePauseAction->setText(i18n("S&top"));
        mResumePauseAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
        mResumePauseAction->setToolTip(i18n("Pause the watching of the current log"));
        mResumePauseAction->setWhatsThis(
            i18n("Pauses the watching of the current log. This action is particularly useful when the system is "
                 "writing too many lines to log files, causing KSystemLog to reload too frequently."));
        mResumePauseAction->setChecked(false);
        actionCollection()->setDefaultShortcut(mResumePauseAction, Qt::CTRL | Qt::Key_P);
    }

    // Be sure that the button will always have a good size
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const auto associatedWidgets{mResumePauseAction->associatedWidgets()};
    for (QWidget *widget : associatedWidgets) {
        if (widget->sizeHint().width() > widget->size().width()) {
            widget->setMinimumSize(widget->sizeHint());
        }
    }
#else
    const auto associatedWidgets{mResumePauseAction->associatedObjects()};
    for (QObject *obj : associatedWidgets) {
        QWidget *widget = qobject_cast<QWidget *>(obj);

        if (widget && (widget->sizeHint().width() > widget->size().width())) {
            widget->setMinimumSize(widget->sizeHint());
        }
    }
#endif
}

void MainWindow::fileOpen()
{
    // Launch the actualizing
    mTabs->load(Globals::instance().findLogMode(QStringLiteral("openLogMode")), mTabs->activeLogManager());
}

void MainWindow::showConfigurationDialog()
{
    qCDebug(KSYSTEMLOG) << "Showing configuration dialog...";
    mConfigurationDialog->showConfiguration();
}

void MainWindow::showLogMessageDialog()
{
    qCDebug(KSYSTEMLOG) << "Launching the Send message dialog box...";

    if (!mLoggedDialog) {
        mLoggedDialog = new LoggerDialog(this);
    }

    mLoggedDialog->initialize();
    mLoggedDialog->exec();
}

void MainWindow::changeStatusBar(const QString &text)
{
    mStatusBar->changeMessage(text);
}

void MainWindow::changeWindowTitle(const QString &text)
{
    // Display this text on the caption
    setCaption(text);
}

void MainWindow::changeCurrentTab()
{
    qCDebug(KSYSTEMLOG) << "Tab has changed";

    LogManager *currentManager = mTabs->activeLogManager();

    if (!currentManager) {
        return;
    }

    // If the tab changed, the selection changes too
    updateSelection();

    // Update the status bar
    updateStatusBar();

    // Updating the current reloading status
    updateReloading();

    bool const enabledReloading = !currentManager->usedView()->logViewWidget()->model()->isProcessingMultipleInsertions();

    bool enabledAction;
    // Change the title of the window
    if (!currentManager->logMode()) {
        changeWindowTitle(i18nc("Newly created tab", "Empty Log"));
        enabledAction = false;
    } else {
        changeWindowTitle(currentManager->title());
        enabledAction = true;
    }

    if (enabledReloading && enabledAction) {
        mReloadAction->setEnabled(true);
        mResumePauseAction->setEnabled(true);
    } else {
        mReloadAction->setEnabled(false);
        mResumePauseAction->setEnabled(false);
    }

    // Update Resume/Pause action state
    if (currentManager->isParsingPaused()) {
        changeResumePauseAction(true);
    } else {
        changeResumePauseAction(false);
    }

    // Updating Detail dialog
    updateDetailDialog();

    qCDebug(KSYSTEMLOG) << "Tab changing done";
}

/**
 * TODO Implements the session restoring
 *
 * The 'config' object points to the session managed
 * config file.  anything you write here will be available
 * later when this app is restored
 */
void MainWindow::saveProperties(KConfigGroup & /*configuration*/)
{
    qCDebug(KSYSTEMLOG) << "Saving properties...";
}

/**
 * TODO Implements the session restoring
 *
 * The 'config' object points to the session managed
 * config file.  this function is automatically called whenever
 * the app is being restored.  read in here whatever you wrote
 * in 'saveProperties'
 */
void MainWindow::readProperties(const KConfigGroup & /*configuration*/)
{
    qCDebug(KSYSTEMLOG) << "Reading properties...";
}

void MainWindow::toggleFilterBar()
{
    qCDebug(KSYSTEMLOG) << "Toggling filter bar..." << mFilterBarAction->isChecked();

    const auto logManagers{mTabs->logManagers()};
    for (LogManager *manager : logManagers) {
        manager->usedView()->toggleLogViewFilter(mFilterBarAction->isChecked());
    }

    KSystemLogConfig::setToggleFilterBar(mFilterBarAction->isChecked());
}

void MainWindow::findNext()
{
    showSearchBar();
    mTabs->activeLogManager()->usedView()->logViewSearch()->findNext();
}

void MainWindow::findPrevious()
{
    showSearchBar();
    mTabs->activeLogManager()->usedView()->logViewSearch()->findPrevious();
}

void MainWindow::showSearchBar()
{
    qCDebug(KSYSTEMLOG) << "Showing search bar...";

    LogManager *activeLogManager = mTabs->activeLogManager();

    const auto logManagers = mTabs->logManagers();
    for (LogManager *manager : logManagers) {
        if (manager != activeLogManager) {
            manager->usedView()->toggleLogViewSearch(true);
        }
    }

    // Be sure to display the view search of the active LogManager at last, and focus to it
    mTabs->activeLogManager()->usedView()->toggleLogViewSearch(true);
}

void MainWindow::setupActions()
{
    qCDebug(KSYSTEMLOG) << "Creating actions...";

    QAction *fileOpenAction = actionCollection()->addAction(KStandardAction::Open, this, SLOT(fileOpen()));
    fileOpenAction->setToolTip(i18n("Open a file in KSystemLog"));
    fileOpenAction->setWhatsThis(i18n("Opens a file in KSystemLog and displays its content in the current tab."));

    mPrintAction = actionCollection()->addAction(KStandardAction::Print);
    mPrintAction->setText(i18n("&Print Selection..."));
    mPrintAction->setToolTip(i18n("Print the selection"));
    mPrintAction->setWhatsThis(
        i18n("Prints the selection. Simply select the important lines and click on this menu entry to print the "
             "selection."));
    mPrintAction->setEnabled(false);

    mPrintPreviewAction = actionCollection()->addAction(KStandardAction::PrintPreview);
    mPrintPreviewAction->setText(i18n("&Print Preview Selection..."));
    mPrintPreviewAction->setToolTip(i18n("Print preview the selection"));
    mPrintPreviewAction->setWhatsThis(
        i18n("Prints preview the selection. Simply select the important lines and click on this menu entry to print the "
             "selection."));
    mPrintPreviewAction->setEnabled(false);

    mSaveAction = actionCollection()->addAction(KStandardAction::SaveAs);
    // TODO Retrieve the system's shortcut of the save action (and not Save as...)
    mSaveAction->setToolTip(i18n("Save the selection to a file"));
    mSaveAction->setWhatsThis(
        i18n("Saves the selection to a file. This action is useful if you want to create an attachment or a "
             "backup of a particular log."));
    mSaveAction->setEnabled(false);
    actionCollection()->setDefaultShortcut(mSaveAction, Qt::CTRL | Qt::Key_S);

    QAction *fileQuitAction = actionCollection()->addAction(KStandardAction::Quit, qApp, SLOT(quit()));
    fileQuitAction->setToolTip(i18n("Quit KSystemLog"));
    fileQuitAction->setWhatsThis(i18n("Quits KSystemLog."));

    mCopyAction = actionCollection()->addAction(KStandardAction::Copy);
    mCopyAction->setToolTip(i18n("Copy the selection to the clipboard"));
    mCopyAction->setWhatsThis(
        i18n("Copies the selection to the clipboard. This action is useful if you want to paste the selection in "
             "a chat or an email."));
    mCopyAction->setEnabled(false);

    mExpandAllAction = actionCollection()->addAction(QStringLiteral("expand_all"));
    mExpandAllAction->setText(i18n("Ex&pand All"));
    mExpandAllAction->setToolTip(i18n("Expand all categories"));
    mExpandAllAction->setWhatsThis(
        i18n("This action opens all main categories. This is enabled only if an option has been selected in the "
             "<b>Group By</b> menu."));
    mExpandAllAction->setEnabled(false);
    actionCollection()->setDefaultShortcut(mExpandAllAction, Qt::CTRL | Qt::Key_X);

    mCollapseAllAction = actionCollection()->addAction(QStringLiteral("collapse_all"));
    mCollapseAllAction->setText(i18n("Col&lapse All"));
    mCollapseAllAction->setToolTip(i18n("Collapse all categories"));
    mCollapseAllAction->setWhatsThis(
        i18n("This action closes all main categories. This is enabled only if an option has been selected in the "
             "<b>Group By</b> menu."));
    mCollapseAllAction->setEnabled(false);
    actionCollection()->setDefaultShortcut(mCollapseAllAction, Qt::CTRL | Qt::Key_L);

    mSendMailAction = actionCollection()->addAction(QStringLiteral("send_mail"));
    mSendMailAction->setText(i18n("&Email Selection..."));
    mSendMailAction->setIcon(QIcon::fromTheme(QStringLiteral("mail-message-new")));
    mSendMailAction->setToolTip(i18n("Send the selection by mail"));
    mSendMailAction->setWhatsThis(
        i18n("Sends the selection by mail. Simply select the important lines and click on this menu entry to send "
             "the selection to a friend or a mailing list."));
    mSendMailAction->setEnabled(false);
    actionCollection()->setDefaultShortcut(mSendMailAction, Qt::CTRL | Qt::Key_M);

    mLogMessageAction = actionCollection()->addAction(QStringLiteral("log_message"), this, SLOT(showLogMessageDialog()));
    mLogMessageAction->setText(i18n("&Add Log Entry..."));
    mLogMessageAction->setIcon(QIcon::fromTheme(QStringLiteral("document-new")));
    mLogMessageAction->setShortcut(Qt::CTRL | Qt::Key_L);
    mLogMessageAction->setToolTip(i18n("Add a log entry to the log system"));
    mLogMessageAction->setWhatsThis(i18n("This action will open a dialog which lets you send a message to the log system."));
    actionCollection()->setDefaultShortcut(mLogMessageAction, Qt::CTRL | Qt::Key_L);

    mSelectAllAction = actionCollection()->addAction(KStandardAction::SelectAll);
    mSelectAllAction->setToolTip(i18n("Select all lines of the current log"));
    mSelectAllAction->setWhatsThis(
        i18n("Selects all lines of the current log. This action is useful if you want, for example, to save all "
             "the content of the current log in a file."));

    mFindAction = actionCollection()->addAction(KStandardAction::Find, this, SLOT(showSearchBar()));
    mFindNextAction = actionCollection()->addAction(KStandardAction::FindNext, this, SLOT(findNext()));
    mFindPreviousAction = actionCollection()->addAction(KStandardAction::FindPrev, this, SLOT(findPrevious()));

    actionCollection()->addAction(KStandardAction::Preferences, this, SLOT(showConfigurationDialog()));

    // TODO Find a solution to display at the right place this action (see Akregator interface)
    mFilterBarAction = actionCollection()->addAction(QStringLiteral("show_quick_filter"), this, SLOT(toggleFilterBar()));
    mFilterBarAction->setText(i18n("Show &Filter Bar"));
    mFilterBarAction->setEnabled(true);
    mFilterBarAction->setCheckable(true);
    mFilterBarAction->setChecked(KSystemLogConfig::toggleFilterBar());

    QAction *newTabAction = actionCollection()->addAction(QStringLiteral("new_tab"), mTabs, SLOT(createTab()));
    newTabAction->setText(i18n("&New Tab"));
    newTabAction->setIcon(QIcon::fromTheme(QStringLiteral("tab-new")));
    newTabAction->setToolTip(i18n("Create a new tab"));
    newTabAction->setWhatsThis(i18n("Creates a new tab which can display another log."));
    mTabs->addAction(newTabAction);
    actionCollection()->setDefaultShortcut(newTabAction, Qt::CTRL | Qt::Key_T);

    QAction *closeTabAction = actionCollection()->addAction(QStringLiteral("close_tab"), mTabs, SLOT(closeTab()));
    closeTabAction->setText(i18n("&Close Tab"));
    closeTabAction->setIcon(QIcon::fromTheme(QStringLiteral("tab-close")));
    closeTabAction->setToolTip(i18n("Close the current tab"));
    closeTabAction->setWhatsThis(i18n("Closes the current tab."));
    mTabs->addAction(closeTabAction);
    actionCollection()->setDefaultShortcut(closeTabAction, Qt::CTRL | Qt::Key_W);

    QAction *duplicateTabAction = actionCollection()->addAction(QStringLiteral("duplicate_tab"), mTabs, SLOT(duplicateTab()));
    duplicateTabAction->setText(i18n("&Duplicate Tab"));
    duplicateTabAction->setIcon(QIcon::fromTheme(QStringLiteral("tab-duplicate")));
    duplicateTabAction->setToolTip(i18n("Duplicate the current tab"));
    duplicateTabAction->setWhatsThis(i18n("Duplicates the current tab."));
    mTabs->addAction(duplicateTabAction);
    actionCollection()->setDefaultShortcut(duplicateTabAction, Qt::SHIFT | Qt::CTRL | Qt::Key_N);

    auto separatorAction = new QAction(this);
    separatorAction->setSeparator(true);
    mTabs->addAction(separatorAction);

    QAction *moveTabLeftAction = actionCollection()->addAction(QStringLiteral("move_tab_left"), mTabs, SLOT(moveTabLeft()));
    moveTabLeftAction->setText(i18n("Move Tab &Left"));
    moveTabLeftAction->setIcon(QIcon::fromTheme(QStringLiteral("arrow-left")));
    moveTabLeftAction->setToolTip(i18n("Move the current tab to the left"));
    moveTabLeftAction->setWhatsThis(i18n("Moves the current tab to the left."));
    mTabs->addAction(moveTabLeftAction);
    actionCollection()->setDefaultShortcut(moveTabLeftAction, Qt::SHIFT | Qt::CTRL | Qt::Key_Left);

    QAction *moveTabRightAction = actionCollection()->addAction(QStringLiteral("move_tab_right"), mTabs, SLOT(moveTabRight()));
    moveTabRightAction->setText(i18n("Move Tab &Right"));
    moveTabRightAction->setIcon(QIcon::fromTheme(QStringLiteral("arrow-right")));
    moveTabRightAction->setToolTip(i18n("Move the current tab to the right"));
    moveTabRightAction->setWhatsThis(i18n("Moves the current tab to the right."));
    mTabs->addAction(moveTabRightAction);
    actionCollection()->setDefaultShortcut(moveTabRightAction, Qt::SHIFT | Qt::CTRL | Qt::Key_Right);

    mReloadAction = actionCollection()->addAction(QStringLiteral("reload"), mTabs, SLOT(reloadCurrent()));
    mReloadAction->setText(i18n("&Reload"));
    mReloadAction->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    mReloadAction->setToolTip(i18n("Reload the current log"));
    mReloadAction->setWhatsThis(i18n("Reloads the current log, if you want to be sure that the view is correctly updated."));
    actionCollection()->setDefaultShortcut(mReloadAction, Qt::Key_F5);

    mResumePauseAction = actionCollection()->addAction(QStringLiteral("resume_pause_parsing"));
    mResumePauseAction->setCheckable(true);
    connect(mResumePauseAction, &QAction::toggled, this, &MainWindow::changeResumePauseAction);
    connect(mResumePauseAction, &QAction::toggled, this, &MainWindow::toggleResumePauseParsing);
    changeResumePauseAction(false);

    mDetailAction = actionCollection()->addAction(QStringLiteral("details"), this, SLOT(showDetailsDialog()));
    mDetailAction->setText(i18n("&Details"));
    mDetailAction->setIcon(QIcon::fromTheme(QStringLiteral("document-preview")));
    mDetailAction->setToolTip(i18n("Display details on the selected line"));
    mDetailAction->setWhatsThis(
        i18n("Displays a dialog box which contains details on the selected line. You are able to navigate through "
             "the logs from this dialog box with the <b>Previous</b> and <b>Next</b> buttons."));
    mDetailAction->setEnabled(false);
    actionCollection()->setDefaultShortcut(mDetailAction, Qt::ALT | Qt::Key_Return);

    mTooltipEnabledAction = actionCollection()->addAction(QStringLiteral("tooltipEnabled"));
    mTooltipEnabledAction->setText(i18n("&Enable Detailed Tooltips"));
    mTooltipEnabledAction->setToolTip(i18n("Disable/Enable the tooltip on the current view"));
    mTooltipEnabledAction->setWhatsThis(i18n("Disables/Enables the tooltip displayed when the cursor hovers a log line."));
    mTooltipEnabledAction->setCheckable(true);
    mTooltipEnabledAction->setChecked(KSystemLogConfig::tooltipEnabled());
    connect(mTooltipEnabledAction, &QAction::toggled, this, &MainWindow::toggleItemTooltip);

    mNewLinesDisplayedAction = actionCollection()->addAction(QStringLiteral("newLinesDisplayed"));
    mNewLinesDisplayedAction->setText(i18n("&Scroll to New Lines"));
    mNewLinesDisplayedAction->setToolTip(i18n("Scrolls or not to the new lines when the log changes"));
    mNewLinesDisplayedAction->setWhatsThis(
        i18n("Scrolls or not to the new lines when the log changes. Check this option if you do not want the "
             "application to scroll automatically at the bottom of the log each time it is refreshed."));
    mNewLinesDisplayedAction->setCheckable(true);
    mNewLinesDisplayedAction->setChecked(KSystemLogConfig::newLinesDisplayed());
    connect(mNewLinesDisplayedAction, &QAction::toggled, this, &MainWindow::toggleNewLinesDisplaying);

    // Toolbar and Menu signals
    connect(mExpandAllAction, &QAction::triggered, mTabs, &TabLogViewsWidget::expandAllCurrentView);
    connect(mCollapseAllAction, &QAction::triggered, mTabs, &TabLogViewsWidget::collapseAllCurrentView);
    connect(mSaveAction, &QAction::triggered, mTabs, &TabLogViewsWidget::fileSaveCurrentView);
    connect(mCopyAction, &QAction::triggered, mTabs, &TabLogViewsWidget::copyToClipboardCurrentView);
    connect(mSendMailAction, &QAction::triggered, mTabs, &TabLogViewsWidget::sendMailCurrentView);
    connect(mPrintAction, &QAction::triggered, mTabs, &TabLogViewsWidget::printSelectionCurrentView);
    connect(mPrintPreviewAction, &QAction::triggered, mTabs, &TabLogViewsWidget::printPreviewSelectionCurrentView);
    connect(mSelectAllAction, &QAction::triggered, mTabs, &TabLogViewsWidget::selectAllCurrentView);
}

void MainWindow::selectLogModeAction(bool)
{
    auto action = qobject_cast<QAction *>(sender());
    auto actionData = action->data().value<ActionData>();
    QString const selectedModeId = actionData.id;

    qCDebug(KSYSTEMLOG) << "Selected action" << selectedModeId;

    LogMode *currentMode = nullptr;
    const auto logModes{Globals::instance().logModes()};
    for (LogMode *logMode : logModes) {
        if (logMode->id() == selectedModeId) {
            currentMode = logMode;
            break;
        }
    }

    if (!currentMode) {
        qCCritical(KSYSTEMLOG) << "The selected mode does not exist";
        return;
    }

    qCDebug(KSYSTEMLOG) << "Selecting " << currentMode->name() << " (" << currentMode->id() << ")";

    mTabs->load(currentMode, mTabs->activeLogManager(), actionData.analyzerOptions);
}

void MainWindow::recreateActions()
{
    unplugActionList(QStringLiteral("log_mode_list"));
    Globals::instance().recreateLogModeActions();
    setupLogActions();
    setupGUI();
    setupLogModeMenu();
}

void MainWindow::setupLogModeMenu()
{
    // Sets up the Logs menu.
    QList<QAction *> menuLogModeActions;
    int serviceItems = 0;
    int othersItems = 0;

    auto servicesAction = new KActionMenu(QIcon::fromTheme(QStringLiteral("preferences-system-session-services")), i18n("Services"), this);
    auto othersAction = new KActionMenu(QIcon::fromTheme(QStringLiteral("preferences-other")), i18n("Others"), this);

    const auto logModeActions{Globals::instance().logModeActions()};
    for (LogModeAction *logModeAction : logModeActions) {
        if (logModeAction->category() == LogModeAction::RootCategory) {
            menuLogModeActions.append(logModeAction->actionMenu());
        } else if (logModeAction->category() == LogModeAction::ServicesCategory) {
            serviceItems++;
            servicesAction->addAction(logModeAction->actionMenu());
        } else if (logModeAction->category() == LogModeAction::OthersCategory) {
            othersAction->addAction(logModeAction->actionMenu());
            othersItems++;
        }
    }

    if (serviceItems) {
        menuLogModeActions.append(servicesAction);
    }

    if (othersItems) {
        menuLogModeActions.append(othersAction);
    }

    // Menu dynamic action list
    unplugActionList(QStringLiteral("log_mode_list"));
    plugActionList(QStringLiteral("log_mode_list"), menuLogModeActions);
}

void MainWindow::setupLogActions()
{
    // Sets up log mode actions.
    const auto logModeActions{Globals::instance().logModeActions()};
    for (LogModeAction *logModeAction : logModeActions) {
        const auto innerActions{logModeAction->innerActions()};
        for (QAction *action : innerActions) {
            auto actionData = action->data().value<ActionData>();
            if (actionData.addToActionCollection) {
                qCDebug(KSYSTEMLOG) << "Adding action" << actionData.id;
                action = actionCollection()->addAction(actionData.id, action);
            }
            connect(action, &QAction::triggered, this, &MainWindow::selectLogModeAction);
        }
    }
}
}

#include "moc_mainWindow.cpp"
