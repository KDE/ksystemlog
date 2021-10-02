/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>

#include <KLocalizedString>

#include "logging.h"

#include "logLine.h"
#include "logMode.h"
#include "logViewWidgetItem.h"

#include "logModeItemBuilder.h"

class LogLine;

class AcpidItemBuilder : public LogModeItemBuilder
{
public:
    AcpidItemBuilder()
    {
    }

    ~AcpidItemBuilder() override
    {
    }

    QString createFormattedText(LogLine *line) const override;
};

