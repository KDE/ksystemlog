/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

// Qt includes
#include <QWidget>

#include "globals.h"

#include "logViewColumn.h"
#include "logViewColumns.h"

class QDropEvent;
class QDragEnterEvent;

class LoadingBar;

class LogViewWidget;
class LogViewSearchWidget;
class LogViewFilterWidget;

class ViewPrivate;

class View : public QWidget
{
    Q_OBJECT

public:
    explicit View(QWidget *parent);

    ~View() override;

    LogViewWidget *logViewWidget() const;

    LoadingBar *loadingBar() const;

    LogViewSearchWidget *logViewSearch() const;

    QSize sizeHint() const override;

public Q_SLOTS:
    void displayLoadingBar(bool display);

    void toggleLogViewFilter(bool display);
    void toggleLogViewSearch(bool display);

protected:
    /**
     * Method which contains the action to do when receiving a drag and drop event
     */
    void dragEnterEvent(QDragEnterEvent *event) override;

    /**
     * Method which accepts
     */
    void dropEvent(QDropEvent *event) override;

private Q_SLOTS:
    void unselectHiddenItems();

Q_SIGNALS:
    void searchFilterChanged();

    void droppedUrls(const QList<QUrl> &urls);

private:
    /*
     * Log view
     */
    LogViewWidget *mLogViewWidget = nullptr;

    /**
     * Filter widget
     */
    LogViewFilterWidget *mLogViewFilterWidget = nullptr;

    /**
     * Search widget
     */
    LogViewSearchWidget *mLogViewSearchWidget = nullptr;

    LoadingBar *mLoadingBar = nullptr;
};

