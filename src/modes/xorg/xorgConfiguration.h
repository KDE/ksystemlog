/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "ksystemlogConfig.h"
#include "logging.h"

#include "xorgLogMode.h"

class XorgConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    XorgConfiguration();

    ~XorgConfiguration() override;

    QStringList xorgPaths() const;

    void setXorgPaths(const QStringList &xorgPaths);

private:
    QStringList mXorgPaths;
};

