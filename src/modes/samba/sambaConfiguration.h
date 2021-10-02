/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "logging.h"

#include "sambaLogMode.h"

#include "ksystemlogConfig.h"

class SambaConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    SambaConfiguration();

    ~SambaConfiguration() override;

    QStringList sambaPaths() const;

    QStringList sambaAccessPaths() const;

    QStringList netbiosPaths() const;

    void setSambaPaths(const QStringList &sambaPaths);

    void setNetbiosPaths(const QStringList &netbiosPaths);

    void setSambaAccessPaths(const QStringList &sambaAccessPaths);

private:
    QStringList mSambaPaths;

    QStringList mSambaAccessPaths;

    QStringList mNetbiosPaths;
};

