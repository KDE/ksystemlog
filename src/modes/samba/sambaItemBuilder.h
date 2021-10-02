/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>

#include "logModeItemBuilder.h"

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "logLine.h"
#include "logMode.h"
#include "logViewWidgetItem.h"

class SambaItemBuilder : public LogModeItemBuilder
{
public:
    SambaItemBuilder()
    {
    }

    ~SambaItemBuilder() override
    {
    }

    QString createFormattedText(LogLine *line) const override;
};

