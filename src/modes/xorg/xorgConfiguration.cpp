/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "xorgConfiguration.h"

XorgConfiguration::XorgConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("XorgLogMode"));

    const QStringList defaultXorgPaths{QStringLiteral("/var/log/Xorg.0.log")};
    mConfiguration->addItemStringList(QStringLiteral("LogFilesPaths"), mXorgPaths, defaultXorgPaths, QStringLiteral("LogFilesPaths"));
}

XorgConfiguration::~XorgConfiguration()
{
}

QStringList XorgConfiguration::xorgPaths() const
{
    return mXorgPaths;
}

void XorgConfiguration::setXorgPaths(const QStringList &xorgPaths)
{
    mXorgPaths = xorgPaths;
}
