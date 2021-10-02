/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "logFile.h"

class LogFileReaderPrivate
{
public:
    virtual ~LogFileReaderPrivate()
    {
    }

    LogFile logFile;
};

