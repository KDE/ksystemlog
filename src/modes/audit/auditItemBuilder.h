#ifndef _AUDIT_ITEM_BUILDER_H_
#define _AUDIT_ITEM_BUILDER_H_

#include "logModeItemBuilder.h"

class LogLine;

class AuditItemBuilder : public LogModeItemBuilder
{
public:
    QString createFormattedText(LogLine *line) const override;

    QString createToolTipText(LogLine *line) const override;
};

#endif // _AUDIT_ITEM_BUILDER_H_
