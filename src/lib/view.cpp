/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "view.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>

#include <QPushButton>

#include <QVBoxLayout>

#include "ksystemlog_debug.h"

#include "logViewWidget.h"

#include "logViewFilterWidget.h"
#include "logViewSearchWidget.h"

#include "logLine.h"

#include "ksystemlogConfig.h"

#include "loadingBar.h"

View::View(QWidget *parent)
    : QWidget(parent)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setContentsMargins(2, 2, 2, 2);
    topLayout->setSpacing(2);

    mLogViewFilterWidget = new LogViewFilterWidget(this);
    connect(mLogViewFilterWidget->filterLine(), &LogViewWidgetSearchLine::treeWidgetUpdated, this, &View::searchFilterChanged);
    connect(mLogViewFilterWidget->filterLine(), &LogViewWidgetSearchLine::treeWidgetUpdated, this, &View::unselectHiddenItems);

    mLogViewFilterWidget->setVisible(KSystemLogConfig::toggleFilterBar());

    topLayout->addWidget(mLogViewFilterWidget);

    mLogViewWidget = new LogViewWidget(this);
    connect(mLogViewWidget, &LogViewWidget::columnsChanged, mLogViewFilterWidget, &LogViewFilterWidget::updateFilterColumns);

    mLogViewFilterWidget->filterLine()->setTreeWidget(mLogViewWidget);
    topLayout->addWidget(mLogViewWidget);

    mLogViewSearchWidget = new LogViewSearchWidget();
    mLogViewSearchWidget->setTreeWidget(mLogViewWidget);

    // The search line is hidden by default
    mLogViewSearchWidget->hide();

    topLayout->addWidget(mLogViewSearchWidget);

    mLoadingBar = new LoadingBar(this);
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

    while (*it) {
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
        qCWarning(KSYSTEMLOG) << "Empty drag and drop";
        return;
    }

    for (const QUrl &url : urls) {
        const QFileInfo fileInfo(url.toLocalFile());

        // TODO Add a recognition of binary files (using the Url mimetype) and refuse them

        if (!fileInfo.isReadable()) {
            qCWarning(KSYSTEMLOG) << "The drag and dropped file is not readable " << url.path();
            return;
        }

        if (fileInfo.isDir()) {
            qCWarning(KSYSTEMLOG) << "Tried to drag and drop a directory " << url.path();
            return;
        }
    }

    // Accept those urls
    event->acceptProposedAction();
}

#include "moc_view.cpp"
