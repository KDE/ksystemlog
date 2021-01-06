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

#ifndef X_SESSION_CONFIGURATION_H
#define X_SESSION_CONFIGURATION_H

#include <QStringList>

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include "xsessionLogMode.h"

#include "ksystemlogConfig.h"

class XSessionConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    XSessionConfiguration();

    ~XSessionConfiguration() override;

    QStringList xorgErrorKeywords() const;

    bool isIgnoreXorgErrors() const;

    void setIgnoreXorgErrors(bool ignore);

    QString xsessionPath() const;

    void setXSessionPath(const QString &xsessionPath);

    QStringList warningKeywords() const;

    QStringList errorKeywords() const;

private:
    QString mXsessionPath;

    bool mIgnoreXorgErrors;

    QStringList mXorgErrorKeywords;

    QStringList mWarningKeywords;
    QStringList mErrorKeywords;
};

#endif // _X_SESSION_CONFIGURATION_H
