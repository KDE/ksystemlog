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

#ifndef CUPS_CONFIGURATION_H
#define CUPS_CONFIGURATION_H

#include <QStringList>

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include "cupsLogMode.h"

#include "ksystemlogConfig.h"

class CupsConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    CupsConfiguration();

    ~CupsConfiguration() override;

    QStringList cupsPaths() const;

    QStringList cupsAccessPaths() const;

    QStringList cupsPagePaths() const;

    QStringList cupsPdfPaths() const;

    void setCupsPaths(const QStringList &cupsPaths);

    void setCupsAccessPaths(const QStringList &cupsAccessPaths);

    void setCupsPagePaths(const QStringList &cupsPagePaths);

    void setCupsPdfPaths(const QStringList &cupsPdfPaths);

private:
    QStringList mCupsPaths;

    QStringList mCupsAccessPaths;

    QStringList mCupsPagePaths;

    QStringList mCupsPdfPaths;
};

#endif // _CUPS_CONFIGURATION_H
