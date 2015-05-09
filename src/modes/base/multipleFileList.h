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

#ifndef _MULTIPLE_FILE_LIST_H_
#define _MULTIPLE_FILE_LIST_H_

#include <QWidget>

#include "fileListHelper.h"

#include "ui_multipleFileListBase.h"


class MultipleFileList : public QWidget, public Ui::MultipleFileListBase {

	Q_OBJECT
	
	public:
		MultipleFileList(QWidget* parent, const QString& descriptionText);
		virtual ~MultipleFileList();
		
		bool isOneOfCategoryEmpty() const;
		
		QStringList paths(int categoryIndex);

		void addPaths(int categoryIndex, const QStringList& paths);
		
		int addCategory(const QString& itemName, const QString& buttonName);
		
	public slots:
		void removeAllItems();
		
	signals:
		void fileListChanged();

	private slots:
		
		void updateButtons();
		
		void removeSelectedItem();
		void moveUpItem();
		void moveDownItem();
		
	protected slots:
		virtual void addItem(int category);
		
		void modifyItem();
		void modifyItem(QTreeWidgetItem* item);
		
	protected:
		void addItemInternal(QTreeWidgetItem* categoryItem, const QString& path);
		void moveItem(int direction);
		void unselectAllItems();
		
		void updateEmptyItems();
		void removeEmptyItems();

		void addEmptyItem(QTreeWidgetItem* item);
		bool isEmptyItem(QTreeWidgetItem* item) const;

		bool isFileListsEmpty() const;
		int categoryCount(int index) const;

		QTreeWidgetItem* findCategoryOfChild(QTreeWidgetItem* childItem);
		
		FileListHelper fileListHelper;
		
		QButtonGroup addButtons;
};

#endif //_MULTIPLE_FILE_LIST_H_
