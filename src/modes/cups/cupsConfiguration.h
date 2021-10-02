/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "logging.h"

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

