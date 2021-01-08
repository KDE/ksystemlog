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

#include "openFactory.h"

#include "logMode.h"
#include "defaults.h"
#include "logFile.h"
#include "logging.h"
#include "ksystemlogConfig.h"

#include "openLogMode.h"
#include "logModeItemBuilder.h"

OpenLogModeFactory::OpenLogModeFactory(QWidget *parent)
    : mParent(parent)
{
}

OpenLogModeFactory::~OpenLogModeFactory()
{
}

QList<LogMode *> OpenLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new OpenLogMode(mParent)};
    return logModes;
}

LogModeAction *OpenLogModeFactory::createLogModeAction() const
{
    return nullptr;
}
