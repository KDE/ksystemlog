/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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

