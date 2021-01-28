#ifndef _AUDIT_LOG_MODE_H_
#define _AUDIT_LOG_MODE_H_

/**
 * Audit Log Mode Identifier
 */
#define AUDIT_LOG_MODE_ID "auditLogMode"

/**
 * Audit Log Icon
 */
#define AUDIT_MODE_ICON "preferences-log"

#include <QList>

#include "logFile.h"
#include "logMode.h"

class AuditLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit AuditLogMode();

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QVector<LogFile> createLogFiles() override;
};

#endif // _AUDIT_LOG_MODE_H_
