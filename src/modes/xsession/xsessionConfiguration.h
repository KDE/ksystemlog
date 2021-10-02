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

