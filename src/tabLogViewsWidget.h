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

#pragma once

#include <QList>
#include <QTabWidget>
#include <QVariant>

class QString;
class QIcon;

class LogManager;
class TabLogManager;
class View;
class LogMode;
class QMenu;

class TabLogViewsWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabLogViewsWidget(QWidget *parent = nullptr);

    ~TabLogViewsWidget() override;

    QList<LogManager *> logManagers() const;

    LogManager *activeLogManager() const;

    void load(LogMode *logMode, LogManager *manager, const QVariant &analyzerOptions = QVariant());

public Q_SLOTS:

    LogManager *createTab();
    LogManager *duplicateTab();

    void closeTab();

    void moveTabLeft();
    void moveTabRight();

    void reloadCurrent();
    void reloadAll();

    /**
     * Display a reload icon on the specified view tab
     */
    void changeReloadingTab(View *view, bool reloading);

    // Methods that transmit a signal to the current logViewWidget
    void expandAllCurrentView();
    void collapseAllCurrentView();
    void selectAllCurrentView();

    void fileSaveCurrentView();
    void copyToClipboardCurrentView();
    void sendMailCurrentView();
    void printSelectionCurrentView();
    void printPreviewSelectionCurrentView();

private Q_SLOTS:
    void changeTab(View *view, const QIcon &icon, const QString &label);

    void changeCurrentTab(int index);
    void changeTitleAddedLines(View *, int);

    void showContextMenu(const QPoint &cursorPosition);
    void showContextMenu(QWidget *tab, const QPoint &cursorPosition);

Q_SIGNALS:

    void logManagerCreated(LogManager *manager);

    void statusBarChanged(const QString &message);

private:
    TabLogManager *newTabLogManager();

    void newTab(View *view);

    LogManager *findRelatedLogManager(View *view);

    QIcon logModeIcon(LogMode *icon) const;

    TabLogManager *activeTabLogManager() const;
    TabLogManager *findRelatedTabLogManager(View *view) const;

    void prepareContextMenu(bool onTab);
    QList<TabLogManager *> mTabLogManagers;

    QMenu *mContextMenu = nullptr;
};

