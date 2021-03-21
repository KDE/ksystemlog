#pragma once

#include "logModeItemBuilder.h"

class LogLine;

class AuditItemBuilder : public LogModeItemBuilder
{
public:
    QString createFormattedText(LogLine *line) const override;

    QString createToolTipText(LogLine *line) const override;
};

