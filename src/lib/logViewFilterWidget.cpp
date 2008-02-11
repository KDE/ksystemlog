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

#include "logViewFilterWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QHBoxLayout>

#include <kcombobox.h>
#include <kiconloader.h>
#include <kicon.h>
#include <klocale.h>

#include "logViewWidget.h"

#include "logViewColumn.h"
#include "logging.h"


class LogViewFilterWidgetPrivate {
public:
	
	LogViewWidgetSearchLine* filterLine;
	
	/**
	 * Filter of the column list
	 */
	KComboBox* filterList;

};


LogViewWidgetSearchLine::LogViewWidgetSearchLine() :
	KTreeWidgetSearchLine() {
	
}

LogViewWidgetSearchLine::~LogViewWidgetSearchLine() {
	
}

void LogViewWidgetSearchLine::updateSearch(const QString& pattern) {
	KTreeWidgetSearchLine::updateSearch(pattern);
	
	emit treeWidgetUpdated();
}


LogViewFilterWidget::LogViewFilterWidget() :
	d(new LogViewFilterWidgetPrivate()) {
	

	QHBoxLayout* filterBarLayout = new QHBoxLayout();
	filterBarLayout->setMargin(0);
	//filterBarLayout->setSpacing(-1);
	setLayout(filterBarLayout);
	
	d->filterLine = new LogViewWidgetSearchLine();
	
	d->filterLine->setToolTip(i18n("Type your filter here"));
	d->filterLine->setWhatsThis(i18n("Allows you to only list items that match the content of this text."));
	d->filterLine->setClickMessage(i18n("Enter your search here..."));
	
	QLabel* filterIcon = new QLabel();
	filterIcon->setPixmap(SmallIcon("view-filter"));
	filterIcon->setBuddy(d->filterLine);
	filterBarLayout->addWidget(filterIcon);
	
	QLabel* filterLabel = new QLabel(i18n("Filter:"));
	filterLabel->setBuddy(d->filterLine);
	filterBarLayout->addWidget(filterLabel);
	
	filterBarLayout->addWidget(d->filterLine);

	initSearchListFilter();

	filterBarLayout->addWidget(d->filterList);
	
}

LogViewFilterWidget::~LogViewFilterWidget() {
	delete d;
}


void LogViewFilterWidget::initSearchListFilter() {
	d->filterList=new KComboBox();

	d->filterList->setToolTip(i18n("Choose the filtered column here"));
	d->filterList->setWhatsThis(i18n("<qt>Allows you to apply the item filter only on the specified column here. \"<i>All</i>\" column means no specific filter.</qt>"));

	d->filterList->addItem(i18n("All"));

	d->filterList->setSizeAdjustPolicy(QComboBox::AdjustToContents);

	connect(d->filterList, SIGNAL(activated(int)), d->filterLine, SLOT(setFocus()));
	connect(d->filterList, SIGNAL(activated(int)), this, SLOT(changeColumnFilter(int)));
	connect(d->filterList, SIGNAL(activated(int)), d->filterLine, SLOT(updateSearch()));

}

void LogViewFilterWidget::updateFilterColumns(const LogViewColumns& columns) {
	logDebug() << "Changing columns..." << endl;
	
	//We first delete all items
	d->filterList->clear();

	//Then we insert the default items
	d->filterList->addItem(i18n("All"));

	foreach(const LogViewColumn& column, columns.columns()) {
		if (column.isFiltered()==true) {
			d->filterList->addItem(column.columnName());
		}
	}

	d->filterList->setCurrentIndex(0);
}

void LogViewFilterWidget::changeColumnFilter(int column) {
	//The user select all columns
	if (column==0) {
		logDebug() << "Searching on all columns" << endl;

		d->filterLine->setSearchColumns(QList<int>());
		return;
	}
	
	logDebug() << "Searching on " << d->filterList->currentIndex() << " column" << endl;
	
	QList<int> filterColumns;
	//currentIndex() - 1 to do not count the "All" columns item
	filterColumns.append(d->filterList->currentIndex() - 1);

	d->filterLine->setSearchColumns(filterColumns);

}

KComboBox* LogViewFilterWidget::filterList() {
	return d->filterList;
}

LogViewWidgetSearchLine* LogViewFilterWidget::filterLine() {
	return d->filterLine;
}

#include "logViewFilterWidget.moc"
