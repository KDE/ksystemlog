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

#include "cupsConfiguration.h"

CupsConfiguration::CupsConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("CupsLogMode"));

    QStringList defaultCupsPaths;
    defaultCupsPaths << QStringLiteral("/var/log/cups/error_log");
    mConfiguration->addItemStringList(QStringLiteral("CupsLogFilesPaths"), mCupsPaths, defaultCupsPaths,
                                      QStringLiteral("CupsLogFilesPaths"));

    QStringList defaultCupsAccessPaths;
    defaultCupsAccessPaths << QStringLiteral("/var/log/cups/access_log");
    mConfiguration->addItemStringList(QStringLiteral("CupsAccessLogFilesPaths"), mCupsAccessPaths,
                                      defaultCupsAccessPaths, QStringLiteral("CupsAccessLogFilesPaths"));

    QStringList defaultCupsPagePaths;
    defaultCupsPagePaths << QStringLiteral("/var/log/cups/page_log");
    mConfiguration->addItemStringList(QStringLiteral("CupsPageLogFilesPaths"), mCupsPagePaths,
                                      defaultCupsPagePaths, QStringLiteral("CupsPageLogFilesPaths"));

    QStringList defaultCupsPdfPaths;
    defaultCupsPdfPaths << QStringLiteral("/var/log/cups/cups-pdf_log");
    mConfiguration->addItemStringList(QStringLiteral("CupsPdfLogFilesPaths"), mCupsPdfPaths,
                                      defaultCupsPdfPaths, QStringLiteral("CupsPdfLogFilesPaths"));
}

CupsConfiguration::~CupsConfiguration()
{
}

QStringList CupsConfiguration::cupsPaths() const
{
    return mCupsPaths;
}

QStringList CupsConfiguration::cupsAccessPaths() const
{
    return mCupsAccessPaths;
}

QStringList CupsConfiguration::cupsPagePaths() const
{
    return mCupsPagePaths;
}

QStringList CupsConfiguration::cupsPdfPaths() const
{
    return mCupsPdfPaths;
}

void CupsConfiguration::setCupsPaths(const QStringList &cupsPaths)
{
    mCupsPaths = cupsPaths;
}

void CupsConfiguration::setCupsAccessPaths(const QStringList &cupsAccessPaths)
{
    mCupsAccessPaths = cupsAccessPaths;
}

void CupsConfiguration::setCupsPagePaths(const QStringList &cupsPagePaths)
{
    mCupsPagePaths = cupsPagePaths;
}

void CupsConfiguration::setCupsPdfPaths(const QStringList &cupsPdfPaths)
{
    mCupsPdfPaths = cupsPdfPaths;
}
