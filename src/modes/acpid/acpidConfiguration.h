/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "ksystemlog_debug.h"

#include "acpidLogMode.h"

#include "ksystemlogConfig.h"

class AcpidConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    AcpidConfiguration();

    ~AcpidConfiguration() override;

    QStringList acpidPaths() const;

    void setAcpidPaths(const QStringList &acpidPaths);

private:
    QStringList mAcpidPaths;
};
