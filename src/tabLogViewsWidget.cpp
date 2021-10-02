/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "tabLogViewsWidget.h"

#include <QMenu>
#include <QPushButton>

#include <KLocalizedString>
#include <QIcon>

#include "logging.h"

#include "logViewExport.h"
#include "view.h"

#include "defaults.h"
#include "logManager.h"
#include "logMode.h"
#include "logViewWidget.h"
#include "tabLogManager.h"

TabLogViewsWidget::TabLogViewsWidget(QWidget *parent)
    : QTabWidget(parent)
{
    auto tabNewTabButton = new QPushButton(QIcon::fromTheme(QStringLiteral("tab-new")), QLatin1String(""), this);
    connect(tabNewTabButton, &QAbstractButton::clicked, this, &TabLogViewsWidget::createTab);

    tabNewTabButton->setToolTip(i18n("Create a new tab"));
    tabNewTabButton->setWhatsThis(i18n("Creates a new tab which can display another log."));

    auto tabCloseTabButton = new QPushButton(QIcon::fromTheme(QStringLiteral("tab-close")), QLatin1String(""), this);
    connect(tabCloseTabButton, &QAbstractButton::clicked, this, &TabLogViewsWidget::closeTab);

    tabCloseTabButton->setToolTip(i18n("Close the current tab"));
    tabCloseTabButton->setWhatsThis(i18n("Closes the current tab."));

    setCornerWidget(tabNewTabButton, Qt::TopLeftCorner);
    setCornerWidget(tabCloseTabButton, Qt::TopRightCorner);

    setUsesScrollButtons(true);

    // The context menu is managed manually
    setContextMenuPolicy(Qt::ActionsContextMenu);

    // connect(this, SIGNAL(mouseDoubleClick()), this, SLOT(createTab()));
    // connect(this, SIGNAL(contextMenu(QPoint)), this, SLOT(showContextMenu(QPoint)));
    // connect(this, SIGNAL(contextMenu(QWidget*,QPoint)), this, SLOT(showContextMenu(QWidget*,QPoint)));

    // TODO Use this (need to connect to movedTab(int, int) signal and update the QList
    // setTabReorderingEnabled(true);

    connect(this, &QTabWidget::currentChanged, this, &TabLogViewsWidget::changeCurrentTab);
}

TabLogViewsWidget::~TabLogViewsWidget()
{
    const QList<TabLogManager *> copy = mTabLogManagers;

    for (TabLogManager *tabLogManager : copy) {
        mTabLogManagers.removeAll(tabLogManager);
        delete tabLogManager;
    }
}

void TabLogViewsWidget::newTab(View *view)
{
    logDebug() << "Inserting to a new tab the view ";

    // Add a tab at the end of the widget
    insertTab(count(), view, QIcon::fromTheme(QStringLiteral(NO_MODE_ICON)), i18n("No Log"));

    tabBar()->setVisible(count() > 1);
}

void TabLogViewsWidget::changeTab(View *view, const QIcon &icon, const QString &label)
{
    logDebug() << "Changing tab " << label;
    const int index = indexOf(view);
    setTabIcon(index, icon);
    setTabText(index, label);
}

QList<LogManager *> TabLogViewsWidget::logManagers() const
{
    QList<LogManager *> logManagers;
    const auto tabLogManagers = mTabLogManagers;
    logManagers.reserve(tabLogManagers.count());
    for (TabLogManager *tabLogManager : tabLogManagers) {
        logManagers.append(tabLogManager->logManager());
    }

    return logManagers;
}

LogManager *TabLogViewsWidget::findRelatedLogManager(View *view)
{
    return findRelatedTabLogManager(view)->logManager();
}

TabLogManager *TabLogViewsWidget::findRelatedTabLogManager(View *view) const
{
    for (TabLogManager *tabLogManager : std::as_const(mTabLogManagers)) {
        if (tabLogManager->logManager()->usedView() == view) {
            return tabLogManager;
        }
    }

    logCritical() << "No log manager found";
    return nullptr;
}

TabLogManager *TabLogViewsWidget::activeTabLogManager() const
{
    View *currentView = static_cast<View *>(currentWidget());

    return findRelatedTabLogManager(currentView);
}

LogManager *TabLogViewsWidget::activeLogManager() const
{
    TabLogManager *tabLogManager = activeTabLogManager();
    if (tabLogManager) {
        return tabLogManager->logManager();
    }
    return nullptr;
}

LogManager *TabLogViewsWidget::createTab()
{
    logDebug() << "Creating a new tab";

    return newTabLogManager()->logManager();
}

void TabLogViewsWidget::moveTabLeft()
{
    logDebug() << "Duplicate tab to the left";

    TabLogManager *currentTabLogManager = activeTabLogManager();
    const int position = indexOf(currentTabLogManager->logManager()->usedView());

    if (position <= 0) {
        logCritical() << "Tab Position <= 0 : " << position;
        return;
    }

    mTabLogManagers.removeAt(position);
    mTabLogManagers.insert(position - 1, currentTabLogManager);

    tabBar()->moveTab(position, position - 1);
}

void TabLogViewsWidget::moveTabRight()
{
    logDebug() << "Duplicate tab to the right";

    TabLogManager *currentTabLogManager = activeTabLogManager();
    const int position = indexOf(currentTabLogManager->logManager()->usedView());

    if (position >= count() - 1) {
        logCritical() << "Tab Position >= count()-1 : " << position;
        return;
    }

    mTabLogManagers.removeAt(position);
    mTabLogManagers.insert(position + 1, currentTabLogManager);

    tabBar()->moveTab(position, position + 1);
}

LogManager *TabLogViewsWidget::duplicateTab()
{
    logDebug() << "Duplicate current tab";

    TabLogManager *currentManager = activeTabLogManager();

    TabLogManager *tabLogManager = newTabLogManager();

    LogMode *mode = currentManager->logManager()->logMode();

    load(mode, tabLogManager->logManager());

    // Returns the newly created LogManager
    return tabLogManager->logManager();
}

TabLogManager *TabLogViewsWidget::newTabLogManager()
{
    logDebug() << "Creating new View...";

    View *view = new View(this);

    logDebug() << "Creating new LogManager...";

    auto logManager = new LogManager(view);

    // Signals from LogManager to Main Class
    connect(logManager, &LogManager::tabTitleChanged, this, &TabLogViewsWidget::changeTab);
    connect(logManager, &LogManager::logUpdated, this, &TabLogViewsWidget::changeTitleAddedLines);

    auto tabLogManager = new TabLogManager(logManager);
    mTabLogManagers.append(tabLogManager);

    logDebug() << "New LogManager created";

    // Finally add the view to the tabs
    newTab(view);

    Q_EMIT logManagerCreated(logManager);

    setCurrentIndex(count() - 1);

    // Set focus to the list
    view->logViewWidget()->setFocus();

    // Returns the newly created TabLogManager
    return tabLogManager;
}

void TabLogViewsWidget::closeTab()
{
    if (count() == 1) {
        logCritical() << "Cannot close tab, one tab left";
        return;
    }

    TabLogManager *currentTabLogManager = activeTabLogManager();

    mTabLogManagers.removeAll(currentTabLogManager);

    removeTab(indexOf(currentTabLogManager->logManager()->usedView()));
    if (count() == 1) {
        tabBar()->hide();
    }

    delete currentTabLogManager;
}

void TabLogViewsWidget::load(LogMode *logMode, LogManager *manager, const QVariant &analyzerOptions)
{
    logDebug() << "Loading a new mode : " << logMode->name();

    if (!manager) {
        logCritical() << "Error while loading a manager ";
        return;
    }

    // The manager is now using the Log mode passed in parameter
    manager->initialize(logMode, analyzerOptions);

    // Launch the reading
    manager->reload();
}

void TabLogViewsWidget::reloadCurrent()
{
    logDebug() << "Reloading current log manager...";

    LogManager *manager = activeLogManager();

    if (manager) {
        manager->reload();
    }
}

void TabLogViewsWidget::reloadAll()
{
    logDebug() << "Reloading all tabs...";

    const auto tabLogManagers = mTabLogManagers;
    for (TabLogManager *tabLogManager : tabLogManagers) {
        // Log manager without log mode does not need to be reloaded
        if (!tabLogManager->logManager()->logMode()) {
            continue;
        }

        // Do a simple reload if it is an open log mode
        if (tabLogManager->logManager()->logMode()->id() == QLatin1String("openLogMode")) {
            tabLogManager->logManager()->reload();
            continue;
        }

        // Do a full loading of other log modes (needed if log files have been modified)
        load(tabLogManager->logManager()->logMode(), tabLogManager->logManager(), tabLogManager->logManager()->analyzerOptions());
    }
}

void TabLogViewsWidget::changeCurrentTab(int index)
{
    logDebug() << "Changing current tab...";

    if (index == -1) {
        return;
    }

    TabLogManager *tabLogManager = activeTabLogManager();

    // Reinit the new lines count since last selection
    tabLogManager->initNewLinesCount();

    // If the tab displayed the new added line count, rename it to the default log mode name
    changeTab(tabLogManager->logManager()->usedView(), logModeIcon(tabLogManager->logManager()->logMode()), tabLogManager->title());

    logDebug() << "Current tab changed";
}

void TabLogViewsWidget::changeReloadingTab(View *view, bool reloading)
{
    TabLogManager *tabLogManager = findRelatedTabLogManager(view);

    if (reloading) {
        changeTab(tabLogManager->logManager()->usedView(), QIcon::fromTheme(QStringLiteral("view-refresh")), tabLogManager->title());
    } else {
        changeTab(tabLogManager->logManager()->usedView(), logModeIcon(tabLogManager->logManager()->logMode()), tabLogManager->title());
    }
}

void TabLogViewsWidget::changeTitleAddedLines(View *view, int addedLinesSinceLastUpdate)
{
    logDebug() << "Changing title" << addedLinesSinceLastUpdate << " added lines...";
    LogManager *currentManager = activeLogManager();

    // Only display added line on tab title if this is not an update of the current tab
    if (currentManager->usedView() != view) {
        TabLogManager *tabLogManager = findRelatedTabLogManager(view);
        tabLogManager->addNewLinesCount(addedLinesSinceLastUpdate);

        // Update the tab title
        changeTab(tabLogManager->logManager()->usedView(), logModeIcon(tabLogManager->logManager()->logMode()), tabLogManager->title());
    }
}

void TabLogViewsWidget::expandAllCurrentView()
{
    activeLogManager()->usedView()->logViewWidget()->expandAll();
}

void TabLogViewsWidget::collapseAllCurrentView()
{
    activeLogManager()->usedView()->logViewWidget()->collapseAll();
}

void TabLogViewsWidget::selectAllCurrentView()
{
    activeLogManager()->usedView()->logViewWidget()->selectAll();
}

void TabLogViewsWidget::fileSaveCurrentView()
{
    LogViewExport logViewExport(this, activeLogManager()->usedView()->logViewWidget());
    connect(&logViewExport, &LogViewExport::statusBarChanged, this, &TabLogViewsWidget::statusBarChanged);
    logViewExport.fileSave();
}

void TabLogViewsWidget::copyToClipboardCurrentView()
{
    LogViewExport logViewExport(this, activeLogManager()->usedView()->logViewWidget());
    connect(&logViewExport, &LogViewExport::statusBarChanged, this, &TabLogViewsWidget::statusBarChanged);
    logViewExport.copyToClipboard();
}

void TabLogViewsWidget::sendMailCurrentView()
{
    LogViewExport logViewExport(this, activeLogManager()->usedView()->logViewWidget());
    connect(&logViewExport, &LogViewExport::statusBarChanged, this, &TabLogViewsWidget::statusBarChanged);
    logViewExport.sendMail();
}

void TabLogViewsWidget::printSelectionCurrentView()
{
    LogViewExport logViewExport(this, activeLogManager()->usedView()->logViewWidget());
    connect(&logViewExport, &LogViewExport::statusBarChanged, this, &TabLogViewsWidget::statusBarChanged);
    logViewExport.printSelection();
}

void TabLogViewsWidget::printPreviewSelectionCurrentView()
{
    LogViewExport logViewExport(this, activeLogManager()->usedView()->logViewWidget());
    connect(&logViewExport, &LogViewExport::statusBarChanged, this, &TabLogViewsWidget::statusBarChanged);
    logViewExport.printPreview();
}

QIcon TabLogViewsWidget::logModeIcon(LogMode *logMode) const
{
    if (!logMode) {
        return QIcon::fromTheme(QStringLiteral(NO_MODE_ICON));
    } else {
        return logMode->icon();
    }
}

void TabLogViewsWidget::prepareContextMenu(bool /*onTab*/)
{
    if (!mContextMenu) {
        mContextMenu = new QMenu(this);
        mContextMenu->addActions(actions());
    }

    // TODO Disable some actions, depending of the onTab value
}

void TabLogViewsWidget::showContextMenu(const QPoint &cursorPosition)
{
    logDebug() << "Showing context menu at " << cursorPosition;

    prepareContextMenu(false);

    mContextMenu->popup(cursorPosition);
}

void TabLogViewsWidget::showContextMenu(QWidget *tab, const QPoint &cursorPosition)
{
    logDebug() << "Showing context menu at " << cursorPosition << " at " << tab->objectName();

    prepareContextMenu(true);

    mContextMenu->popup(cursorPosition);
}
