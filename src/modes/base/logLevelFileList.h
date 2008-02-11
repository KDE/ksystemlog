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

#ifndef _LOG_LEVEL_FILE_LIST_H_
#define _LOG_LEVEL_FILE_LIST_H_

#include "fileList.h"

class QPushButton;
class LogLevel;


class LogLevelFileList : public FileList {

	Q_OBJECT
	
	public:
		LogLevelFileList(QWidget* parent, const QString& description);
		
		virtual ~LogLevelFileList();

		QList<int> levels();
		
		void addPaths(const QStringList& filePaths, const QList<int>& fileLevels);

	private:
		LogLevel* level(int i);
		
		void insertItem(LogLevel* level, const QString& itemText);
		
		static int LogLevelRole;
		
		QPushButton* changeItem;
		
	protected slots:
		void addItem();
		
	private slots:
		void updateSpecificButtons();

		
		void changeItemType();
	
	    

};

#endif //_LOG_LEVEL_FILE_LIST_H_
