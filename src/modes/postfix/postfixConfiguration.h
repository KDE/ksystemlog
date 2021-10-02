/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include <QList>
#include <QStringList>

#include "defaults.h"
#include "genericConfiguration.h"
#include "globals.h"

#include "postfixLogMode.h"

class PostfixConfiguration : public GenericLogModeConfiguration
{
    Q_OBJECT

public:
    PostfixConfiguration();

    ~PostfixConfiguration() override
    {
    }
};

