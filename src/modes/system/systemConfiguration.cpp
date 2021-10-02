/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "systemConfiguration.h"

SystemConfiguration::SystemConfiguration()
    : GenericLogModeConfiguration(QStringLiteral(SYSTEM_LOG_MODE_ID),
                                  QStringList() << QStringLiteral("/var/log/syslog"),
                                  QList<int>() << Globals::INFORMATION_LOG_LEVEL_ID)
{
    /*
        <default>/var/log/messages,/var/log/syslog,/var/log/debug</default>
        <default>2,2,1</default>
        */
}
