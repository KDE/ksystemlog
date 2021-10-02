/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "logLine.h"
#include "logMode.h"
#include "logViewWidgetItem.h"

#include "logModeItemBuilder.h"

class LogLine;

class XorgItemBuilder : public LogModeItemBuilder
{
public:
    XorgItemBuilder()
    {
    }

    ~XorgItemBuilder() override
    {
    }

    void prepareItem(LogViewWidgetItem *item) const override;

    QString createToolTipText(LogLine *line) const override;

    QString createFormattedText(LogLine *line) const override;
};

