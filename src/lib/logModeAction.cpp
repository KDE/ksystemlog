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

#include "logModeAction.h"

class LogModeActionPrivate {
public:
	bool inToolBar;
	
	LogModeAction::Category category;
};

LogModeAction::LogModeAction() :
	d(new LogModeActionPrivate()) {
	
	d->inToolBar = true;
	d->category = LogModeAction::RootCategory;
}

LogModeAction::~LogModeAction() {
	delete d;
}

void LogModeAction::setInToolBar(bool inToolBar) {
	d->inToolBar = inToolBar;
}

bool LogModeAction::isInToolBar() {
	return d->inToolBar;
}

void LogModeAction::setCategory(LogModeAction::Category category) {
	d->category = category;
}

LogModeAction::Category LogModeAction::category() {
	return d->category;
}
