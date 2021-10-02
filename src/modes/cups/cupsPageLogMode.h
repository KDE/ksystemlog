/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

/**
 * Cups Page Log Mode Identifier
 */
#define CUPS_PAGE_LOG_MODE_ID "cupsPageLogMode"

/**
 * Cups Page Log Icon
 */
#define CUPS_PAGE_MODE_ICON "document-new"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class CupsConfiguration;
class CupsConfigurationWidget;

class CupsPageLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit CupsPageLogMode(QSharedPointer<CupsConfiguration> &cupsConfiguration, CupsConfigurationWidget *cupsConfigurationWidget);

    ~CupsPageLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

