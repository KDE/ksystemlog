/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "postfixConfiguration.h"

PostfixConfiguration::PostfixConfiguration()
    : GenericLogModeConfiguration(QStringLiteral(POSTFIX_LOG_MODE_ID),
                                  QStringList() << QStringLiteral("/var/log/mail.log") << QStringLiteral("/var/log/mail.info")
                                                << QStringLiteral("/var/log/mail.warn") << QStringLiteral("/var/log/mail.err"),
                                  QList<int>() << Globals::NOTICE_LOG_LEVEL_ID << Globals::INFORMATION_LOG_LEVEL_ID << Globals::WARNING_LOG_LEVEL_ID
                                               << Globals::ERROR_LOG_LEVEL_ID)
{
}
