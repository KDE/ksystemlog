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

#include "view.h"

#include <QDropEvent>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QMimeData>

#include <QPushButton>

#include <QVBoxLayout>

#include "logging.h"

#include "logViewWidget.h"

#include "logViewFilterWidget.h"
#include "logViewSearchWidget.h"

#include "logLine.h"

#include "ksystemlogConfig.h"

#include "loadingBar.h"

View::View(QWidget *parent)
    : QWidget(parent)
{
    auto *topLayout = new QVBoxLayout(this);
    topLayout->setContentsMargins(2, 2, 2, 2);
    topLayout->setSpacing(2);

    mLogViewFilterWidget = new LogViewFilterWidget();
    connect(mLogViewFilterWidget->filterLine(), &LogViewWidgetSearchLine::treeWidgetUpdated, this,
            &View::searchFilterChanged);
    connect(mLogViewFilterWidget->filterLine(), &LogViewWidgetSearchLine::treeWidgetUpdated, this,
            &View::unselectHiddenItems);

    mLogViewFilterWidget->setVisible(KSystemLogConfig::toggleFilterBar());

    topLayout->addWidget(mLogViewFilterWidget);

    mLogViewWidget = new LogViewWidget(this);
    connect(mLogViewWidget, &LogViewWidget::columnsChanged, mLogViewFilterWidget,
            &LogViewFilterWidget::updateFilterColumns);

    mLogViewFilterWidget->filterLine()->setTreeWidget(mLogViewWidget);
    topLayout->addWidget(mLogViewWidget);

    mLogViewSearchWidget = new LogViewSearchWidget();
    mLogViewSearchWidget->setTreeWidget(mLogViewWidget);

    // The search line is hidden by default
    mLogViewSearchWidget->hide();

    topLayout->addWidget(mLogViewSearchWidget);

    mLoadingBar = new LoadingBar();
    connect(mLoadingBar, &LoadingBar::displayed, this, &View::displayLoadingBar);

    topLayout->addWidget(mLoadingBar);
    mLoadingBar->hide();

    // Accept Drag and Drop
    setAcceptDrops(true);
}

View::~View()
{
}

LogViewWidget *View::logViewWidget() const
{
    return mLogViewWidget;
}

LoadingBar *View::loadingBar() const
{
    return mLoadingBar;
}

void View::displayLoadingBar(bool display)
{
    if (display) {
        mLogViewWidget->hide();
        mLogViewSearchWidget->hide();
        mLogViewFilterWidget->hide();

        mLoadingBar->show();
    } else {
        mLogViewWidget->show();
        mLogViewFilterWidget->setVisible(KSystemLogConfig::toggleFilterBar());
        mLogViewFilterWidget->filterLine()->updateSearch();
        // No need to redisplay the search bar

        mLoadingBar->hide();
    }
}

void View::toggleLogViewFilter(bool display)
{
    if (display) {
        mLogViewFilterWidget->show();
    } else {
        mLogViewFilterWidget->filterLine()->clear();
        mLogViewFilterWidget->hide();
    }
}

void View::toggleLogViewSearch(bool display)
{
    if (display) {
        mLogViewSearchWidget->displaySearch();
    } else {
        mLogViewSearchWidget->hide();
    }
}

LogViewSearchWidget *View::logViewSearch() const
{
    return mLogViewSearchWidget;
}

void View::unselectHiddenItems()
{
    QTreeWidgetItemIterator it(mLogViewWidget, QTreeWidgetItemIterator::Selected);

    while (*it != nullptr) {
        QTreeWidgetItem *item = *it;

        if (item->isHidden()) {
            item->setSelected(false);
        }

        ++it;
    }
}

QSize View::sizeHint() const
{
    return {500, 500};
}

void View::dropEvent(QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();

    // If URLs have been dropped
    if (!urls.isEmpty()) {
        Q_EMIT droppedUrls(urls);
    }
}

void View::dragEnterEvent(QDragEnterEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();

    // If URLs have been dropped
    if (urls.isEmpty()) {
        logWarning() << "Empty drag and drop";
        return;
    }

    for (const QUrl &url : urls) {
        const QFileInfo fileInfo(url.toLocalFile());

        // TODO Add a recognition of binary files (using the Url mimetype) and refuse them

        if (!fileInfo.isReadable()) {
            logWarning() << "The drag and dropped file is not readable " << url.path();
            return;
        }

        if (fileInfo.isDir()) {
            logWarning() << "Tried to drag and drop a directory " << url.path();
            return;
        }
    }

    // Accept those urls
    event->acceptProposedAction();
}
