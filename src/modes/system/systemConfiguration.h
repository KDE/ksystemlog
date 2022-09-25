/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "defaults.h"
#include "genericConfiguration.h"
#include "globals.h"

#include "systemLogMode.h"

class SystemConfiguration : public GenericLogModeConfiguration
{
    Q_OBJECT

public:
    SystemConfiguration();

    ~SystemConfiguration() override
    {
    }
};
