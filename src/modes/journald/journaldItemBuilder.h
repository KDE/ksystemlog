/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include "logModeItemBuilder.h"

class LogLine;

class JournaldItemBuilder : public LogModeItemBuilder
{
public:
    QString createFormattedText(LogLine *line) const override;

    QString createToolTipText(LogLine *line) const override;
};

