/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "syslogAnalyzer.h"

#include "ksystemlog_debug.h"
#include "logFile.h"

#include "openLogMode.h"

class OpenAnalyzer : public SyslogAnalyzer
{
    Q_OBJECT

public:
    explicit OpenAnalyzer(LogMode *logMode);

    ~OpenAnalyzer() override
    {
    }
};
