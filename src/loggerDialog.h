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

#ifndef _LOGGER_DIALOG_H_
#define _LOGGER_DIALOG_H_

#include <QWidget>


#include "ui_loggerDialogBase.h"

class LoggerDialogPrivate;

class LoggerDialog: public QDialog, public Ui::LoggerDialogBase {
	Q_OBJECT
	public:
		explicit LoggerDialog(QWidget* parent = NULL);
	
		virtual ~LoggerDialog();
		
		void initialize();
		
	protected slots:
		void sendMessage();
		
		void textChanged();
		
		void changeTagActivation(bool activation);
		void changeFileActivation(bool activation);
		void changeMessageActivation(bool activation);
		
	private:
		void buildMaps();
		
		LoggerDialogPrivate* const d;

};

#endif // _LOGGER_DIALOG_H_
