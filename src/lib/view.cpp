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

#include <QWidget>

#include <QDropEvent>
#include <QDragEnterEvent>
#include <QFileInfo>

#include <QPushButton>

#include <QVBoxLayout>

#include <kiconloader.h>
#include <kicon.h>
#include <klocale.h>

#include "logging.h"

#include "logViewWidget.h"

#include "logViewFilterWidget.h"
#include "logViewSearchWidget.h"

#include "logLine.h"
#include "logViewColumn.h"
#include "logViewColumns.h"

#include "ksystemlogConfig.h"

#include "loadingBar.h"

class ViewPrivate {

public:
	/*
	 * Log view
	 */
	LogViewWidget* logViewWidget;

	/**
	 * Filter widget
	 */
	LogViewFilterWidget* logViewFilterWidget;

	/**
	 * Search widget
	 */
	LogViewSearchWidget* logViewSearchWidget;

	LoadingBar* loadingBar;
};

View::View(QWidget *parent) :
	QWidget(parent), 
	d(new ViewPrivate()) {

	d->logViewWidget = NULL;

	QVBoxLayout* topLayout = new QVBoxLayout();
	topLayout->setMargin(2);
	topLayout->setSpacing(2);
	this->setLayout(topLayout);

	d->logViewFilterWidget = new LogViewFilterWidget();
	connect(d->logViewFilterWidget->filterLine(), SIGNAL(treeWidgetUpdated()), this, SIGNAL(searchFilterChanged()));
	connect(d->logViewFilterWidget->filterLine(), SIGNAL(treeWidgetUpdated()), this, SLOT(unselectHiddenItems()));
	
	d->logViewFilterWidget->setVisible(KSystemLogConfig::toggleFilterBar());

	topLayout->addWidget(d->logViewFilterWidget);
	
	d->logViewWidget=new LogViewWidget(this);
	connect(d->logViewWidget, SIGNAL(columnsChanged(const LogViewColumns&)), d->logViewFilterWidget, SLOT(updateFilterColumns(const LogViewColumns&)));
	
	d->logViewFilterWidget->filterLine()->setTreeWidget(d->logViewWidget);
	topLayout->addWidget(d->logViewWidget);
	

	d->logViewSearchWidget = new LogViewSearchWidget();
	d->logViewSearchWidget->setTreeWidget(d->logViewWidget);
	
	//The search line is hidden by default
	d->logViewSearchWidget->hide();

	topLayout->addWidget(d->logViewSearchWidget);
	
	d->loadingBar = new LoadingBar();
	connect(d->loadingBar, SIGNAL(displayed(bool)), this, SLOT(displayLoadingBar(bool)));

	topLayout->addWidget(d->loadingBar);
	d->loadingBar->hide();
	
	//Accept Drag and Drop
	setAcceptDrops(true);

}

View::~View() {
	//All widgets are deleted automatically by Qt
	
	delete d;
}

LogViewWidget* View::logViewWidget() const {
	return d->logViewWidget;
}

LoadingBar* View::loadingBar() const {
	return d->loadingBar;
}

void View::displayLoadingBar(bool display) {
	if (display == true) {
		d->logViewWidget->hide();
		d->logViewSearchWidget->hide();
		d->logViewFilterWidget->hide();
		
		d->loadingBar->show();
	}
	else {
		d->logViewWidget->show();
		d->logViewFilterWidget->setVisible(KSystemLogConfig::toggleFilterBar());
		//No need to redisplay the search bar

		d->loadingBar->hide();
	}
	
}

void View::toggleLogViewFilter(bool display) {
	if (display == true)
		d->logViewFilterWidget->show();
	else
		d->logViewFilterWidget->hide();
}

void View::toggleLogViewSearch(bool display) {
	if (display == true)
		d->logViewSearchWidget->displaySearch();
	else
		d->logViewSearchWidget->hide();
}

LogViewSearchWidget* View::logViewSearch() const {
	return d->logViewSearchWidget;
}

void View::unselectHiddenItems() {
	QTreeWidgetItemIterator it(d->logViewWidget, QTreeWidgetItemIterator::Selected);

	while ( *it != NULL) {
		QTreeWidgetItem* item = *it;
		
		if (item->isHidden()) {
			item->setSelected(false);
		}
		
		++it;
	}
}

QSize View::sizeHint() const {
	return QSize(500, 500);
}

void View::dropEvent(QDropEvent* event) {
	KUrl::List urls = KUrl::List::fromMimeData( event->mimeData() );
	
	//If URLs have been dropped
	if ( ! urls.isEmpty() ) {

		emit droppedUrls(urls);
	}
}

void View::dragEnterEvent(QDragEnterEvent* event) {
	KUrl::List urls = KUrl::List::fromMimeData( event->mimeData() );
	
	//If URLs have been dropped
	if (urls.isEmpty() ) {
		logWarning() << "Empty drag and drop" << endl;
		return;
	}
	
	foreach (const KUrl &url, urls) {
		QFileInfo fileInfo(url.path());
		
		//TODO Add a recognition of binary files (using the Url mimetype) and refuse them
		
		if (fileInfo.isReadable() == false) {
			logWarning() << "The drag and dropped file is not readable " << url.path() << endl;
			return;
		}
		
		if (fileInfo.isDir()) {
			logWarning() << "Tried to drag and drop a directory " << url.path() << endl;
			return;
		}
	}

	//Accept those urls
	event->acceptProposedAction();

}

#include "view.moc"
