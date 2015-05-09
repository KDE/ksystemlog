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

#include "detailDialog.h"

//Qt includes

#include <QIcon>
#include <kiconloader.h>
#include <KLocalizedString>
#include <KGuiItem>
#include <KStandardGuiItem>

#include "logViewWidget.h"
#include "logViewWidgetItem.h"
#include "logLine.h"

#include "logging.h"

DetailDialog::DetailDialog(QWidget* parent) : 
	QDialog(parent),
	logViewWidget(NULL) {
	
	setupUi(this);
	
	previous->setText(i18n("&Previous"));
	previous->setIcon(QIcon::fromTheme( QLatin1String( "arrow-up" )));
	connect(previous, SIGNAL(clicked()), this, SLOT(previousItem()));
	
	next->setText(i18n("&Next"));
	next->setIcon(QIcon::fromTheme( QLatin1String( "arrow-down" )));
	connect(next, SIGNAL(clicked()), this, SLOT(nextItem()));
	
	KGuiItem::assign(closeButton,KStandardGuiItem::close());
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	
}

DetailDialog::~DetailDialog() {

}

void DetailDialog::selectionChanged(LogViewWidget* logViewWidget) {
	this->logViewWidget=logViewWidget;
	
	updateDetails();
}

//TODO Try to find a method that reload (an resize) correctly the content of the detail dialog
void DetailDialog::updateDetails() {
  //logDebug() << "Updating Detail Dialog...";
	
	//Get the current-last item selected
	LogViewWidgetItem* item=logViewWidget->lastSelectedItem();
	if (item==NULL) {
    logDebug() << "No item found.";
		return;
	}
	
	icon->setPixmap(DesktopIcon(item->logLine()->logLevel()->icon()));
	
	header->setText(item->logLine()->formattedText());
	
	message->setText(item->logLine()->logItems().last());
	
	if (logViewWidget->topLevelItem(logViewWidget->indexOfTopLevelItem(item) - 1)==NULL)
		previous->setEnabled(false);
	else
		previous->setEnabled(true);
	
	if (logViewWidget->topLevelItem(logViewWidget->indexOfTopLevelItem(item) + 1 )==NULL)
		next->setEnabled(false);
	else
		next->setEnabled(true);
		
	/*
	header->adjustSize();
	this->adjustSize();
	*/
}

void DetailDialog::moveToItem(int direction) {
	if (direction < 0)
    logDebug() << "Go to previous item...";
	else
    logDebug() << "Go to next item...";
	
	//Get the current-last item selected
	LogViewWidgetItem* item=logViewWidget->lastSelectedItem();
	if (item==NULL) {
    logDebug() << "No item found.";
		return;
	}
	
	QTreeWidgetItem* destinationItem = logViewWidget->topLevelItem( logViewWidget->indexOfTopLevelItem(item) + direction );
	if (destinationItem==NULL) {
		if (direction < 0)
      logDebug() << "No previous item found.";
		else
      logDebug() << "No next item found.";
		return;
	}
	
	logViewWidget->setItemSelected(item, false);
	logViewWidget->setItemSelected(destinationItem, true);
	logViewWidget->scrollToItem(destinationItem);
	
	updateDetails();
}

void DetailDialog::previousItem() {
	moveToItem(-1);
}

void DetailDialog::nextItem() {
	moveToItem(1);
}


