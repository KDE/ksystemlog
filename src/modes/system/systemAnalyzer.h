/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include "syslogAnalyzer.h"

#include "systemLogMode.h"

class SystemAnalyzer : public SyslogAnalyzer
{
    Q_OBJECT

public:
    explicit SystemAnalyzer(LogMode *logMode);

    ~SystemAnalyzer() override
    {
    }
};

