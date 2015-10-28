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

#ifndef _CUPS_CONFIGURATION_H_
#define _CUPS_CONFIGURATION_H_

#include <QStringList>

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include "cupsLogMode.h"

#include "ksystemlogConfig.h"

class CupsConfigurationPrivate
{
public:
    QStringList cupsPaths;

    QStringList cupsAccessPaths;

    QStringList cupsPagePaths;

    QStringList cupsPdfPaths;
};

class CupsConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    CupsConfiguration()
        : d(new CupsConfigurationPrivate())
    {
        configuration->setCurrentGroup(QStringLiteral("CupsLogMode"));

        QStringList defaultCupsPaths;
        defaultCupsPaths << QStringLiteral("/var/log/cups/error_log");
        configuration->addItemStringList(QStringLiteral("CupsLogFilesPaths"), d->cupsPaths, defaultCupsPaths,
                                         QStringLiteral("CupsLogFilesPaths"));

        QStringList defaultCupsAccessPaths;
        defaultCupsAccessPaths << QStringLiteral("/var/log/cups/access_log");
        configuration->addItemStringList(QStringLiteral("CupsAccessLogFilesPaths"), d->cupsAccessPaths,
                                         defaultCupsAccessPaths, QStringLiteral("CupsAccessLogFilesPaths"));

        QStringList defaultCupsPagePaths;
        defaultCupsPagePaths << QStringLiteral("/var/log/cups/page_log");
        configuration->addItemStringList(QStringLiteral("CupsPageLogFilesPaths"), d->cupsPagePaths,
                                         defaultCupsPagePaths, QStringLiteral("CupsPageLogFilesPaths"));

        QStringList defaultCupsPdfPaths;
        defaultCupsPdfPaths << QStringLiteral("/var/log/cups/cups-pdf_log");
        configuration->addItemStringList(QStringLiteral("CupsPdfLogFilesPaths"), d->cupsPdfPaths,
                                         defaultCupsPdfPaths, QStringLiteral("CupsPdfLogFilesPaths"));
    }

    virtual ~CupsConfiguration() { delete d; }

    QStringList cupsPaths() const { return d->cupsPaths; }

    QStringList cupsAccessPaths() const { return d->cupsAccessPaths; }

    QStringList cupsPagePaths() const { return d->cupsPagePaths; }

    QStringList cupsPdfPaths() const { return d->cupsPdfPaths; }

    void setCupsPaths(const QStringList &cupsPaths) { d->cupsPaths = cupsPaths; }

    void setCupsAccessPaths(const QStringList &cupsAccessPaths) { d->cupsAccessPaths = cupsAccessPaths; }

    void setCupsPagePaths(const QStringList &cupsPagePaths) { d->cupsPagePaths = cupsPagePaths; }

    void setCupsPdfPaths(const QStringList &cupsPdfPaths) { d->cupsPdfPaths = cupsPdfPaths; }

private:
    CupsConfigurationPrivate *const d;
};

#endif // _CUPS_CONFIGURATION_H_
