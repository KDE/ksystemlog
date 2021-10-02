/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "acpidConfiguration.h"

AcpidConfiguration::AcpidConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("AcpidLogMode"));

    const QStringList defaultAcpidPaths{QStringLiteral("/var/log/acpid")};
    mConfiguration->addItemStringList(QStringLiteral("LogFilesPaths"), mAcpidPaths, defaultAcpidPaths, QStringLiteral("LogFilesPaths"));
}

AcpidConfiguration::~AcpidConfiguration()
{
}

QStringList AcpidConfiguration::acpidPaths() const
{
    return mAcpidPaths;
}

void AcpidConfiguration::setAcpidPaths(const QStringList &acpidPaths)
{
    mAcpidPaths = acpidPaths;
}
