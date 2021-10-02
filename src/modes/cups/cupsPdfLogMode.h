/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

/**
 * Cups Pdf Log Mode Identifier
 */
#define CUPS_PDF_LOG_MODE_ID "cupsPdfLogMode"

/**
 * Cups Pdf Log Icon
 */
#define CUPS_PDF_MODE_ICON "application-pdf"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class CupsConfiguration;
class CupsConfigurationWidget;

class CupsPdfLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit CupsPdfLogMode(QSharedPointer<CupsConfiguration> &cupsConfiguration, CupsConfigurationWidget *cupsConfigurationWidget);

    ~CupsPdfLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

