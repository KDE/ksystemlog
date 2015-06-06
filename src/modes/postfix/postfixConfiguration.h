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

#ifndef _POSTFIX_CONFIGURATION_H_
#define _POSTFIX_CONFIGURATION_H_

#include <QStringList>
#include <QList>

#include "genericConfiguration.h"
#include "globals.h"
#include "defaults.h"

#include "postfixLogMode.h"

class PostfixConfiguration : public GenericLogModeConfiguration
{
    Q_OBJECT

public:
    PostfixConfiguration()
        : GenericLogModeConfiguration(
              QLatin1String(POSTFIX_LOG_MODE_ID),
              QStringList() << QLatin1String("/var/log/mail.log") << QLatin1String("/var/log/mail.info")
                            << QLatin1String("/var/log/mail.warn") << QLatin1String("/var/log/mail.err"),
              QList<int>() << NOTICE_LOG_LEVEL_ID << INFORMATION_LOG_LEVEL_ID << WARNING_LOG_LEVEL_ID
                           << ERROR_LOG_LEVEL_ID)
    {
    }

    virtual ~PostfixConfiguration() {}
};

#endif // _POSTFIX_CONFIGURATION_H_
