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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

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

#endif // MAIN_WINDOW_H
