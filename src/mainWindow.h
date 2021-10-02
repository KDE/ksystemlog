/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

// KDE includes
#include <KConfig>
#include <kxmlguiwindow.h>

#include "logModeAction.h"

class LogManager;
class LogMode;

class View;

class TabLogViewsWidget;
class QPrinter;
class DetailDialog;
class LoggerDialog;
class ConfigurationDialog;
namespace KSystemLog
{
class StatusBar;
/**
 * This class serves as the main window for ksystemlog.  It handles the
 * menus, toolbars, and status bars.
 */
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    /**
     * Default Constructor
     */
    MainWindow();

    /**
     * Default Destructor
     */
    ~MainWindow() override;

    TabLogViewsWidget *tabs();

protected:
    /**
     * This function is called when it is time for the app to save its
     * properties for session management purposes.
     */
    void saveProperties(KConfigGroup &configuration) override;

    /**
     * This function is called when this app is restored.  The KConfig
     * object points to the session management config file that was saved
     * with @ref saveProperties
     */
    void readProperties(const KConfigGroup &configuration) override;

    /**
     * Reimplemented to save configuration when closing.
     */
    void closeEvent(QCloseEvent *event) override;

public Q_SLOTS:
    void changeStatusBar(const QString &text);
    void changeWindowTitle(const QString &text);

    void updateStatusBar();

    void prepareCreatedLogManager(LogManager *logManager);

private Q_SLOTS:
    void fileOpen();

    void showConfigurationDialog();
    void showDetailsDialog();
    void showLogMessageDialog();

    // Transmits signals to active LogManager
    void showSearchBar();
    void findNext();
    void findPrevious();

    void updateSelection();
    void updateReloading();

    void toggleFilterBar();

    void toggleItemTooltip(bool enabled);
    void toggleNewLinesDisplaying(bool displayed);
    void toggleResumePauseParsing(bool paused);

    void changeCurrentTab();

    void changeResumePauseAction(bool paused);
    void selectLogModeAction(bool);
    void recreateActions();

private:
    void loadLogModePlugins();

    void setupStatusBar();

    void setupTabLogViews();

    void setupActions();
    void setupLogModeMenu();
    void setupLogActions();

    void updateDetailDialog();

    QAction *mSaveAction = nullptr;
    QAction *mCopyAction = nullptr;

    QAction *mReloadAction = nullptr;

    QAction *mSendMailAction = nullptr;
    QAction *mLogMessageAction = nullptr;

    QAction *mFilterBarAction = nullptr;

    QAction *mSelectAllAction = nullptr;

    QAction *mExpandAllAction = nullptr;
    QAction *mCollapseAllAction = nullptr;

    QAction *mResumePauseAction = nullptr;
    QAction *mDetailAction = nullptr;
    QAction *mPrintAction = nullptr;
    QAction *mPrintPreviewAction = nullptr;

    QAction *mFindAction = nullptr;
    QAction *mFindNextAction = nullptr;
    QAction *mFindPreviousAction = nullptr;

    QAction *mTooltipEnabledAction = nullptr;
    QAction *mNewLinesDisplayedAction = nullptr;

    /**
     * Action groups which stores all Log Mode Actions
     */
    QActionGroup *mLogModesActionGroup = nullptr;

    QPrinter *mPrinter = nullptr;

    /**
     * Detail dialog
     */
    DetailDialog *mDetailDialog = nullptr;

    /**
     * Logged Dialog
     */
    LoggerDialog *mLoggedDialog = nullptr;

    ConfigurationDialog *mConfigurationDialog = nullptr;

    /**
     * Tab widget managing different views
     */
    TabLogViewsWidget *mTabs = nullptr;

    KSystemLog::StatusBar *mStatusBar = nullptr;
};
}

