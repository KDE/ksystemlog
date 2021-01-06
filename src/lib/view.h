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

#ifndef VIEW_H
#define VIEW_H

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

#endif // VIEW_H
