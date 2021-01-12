#include "auditConfiguration.h"

#include "auditLogMode.h"

AuditConfiguration::AuditConfiguration()
    : GenericLogModeConfiguration(QStringLiteral(AUDIT_LOG_MODE_ID),
                                  {QStringLiteral("/var/log/audit/audit.log")}, {2})
{
}
