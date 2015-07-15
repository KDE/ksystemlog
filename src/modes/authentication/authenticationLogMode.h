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

#ifndef _AUTHENTICATION_LOG_MODE_H_
#define _AUTHENTICATION_LOG_MODE_H_

/**
 * Acpid Log Mode Identifier
 */
#define AUTHENTICATION_LOG_MODE_ID "authenticationLogMode"

/**
 * Authentication Log Icon
 */
#define AUTHENTICATION_MODE_ICON "document-encrypt"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class AuthenticationLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit AuthenticationLogMode();

    ~AuthenticationLogMode();

    QList<LogFile> createLogFiles();
};

#endif // _AUTHENTICATION_LOG_MODE_H_
