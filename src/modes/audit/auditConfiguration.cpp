#include "auditConfiguration.h"

#include "auditLogMode.h"

#include "globals.h"

AuditConfiguration::AuditConfiguration()
    : GenericLogModeConfiguration(QStringLiteral(AUDIT_LOG_MODE_ID),
                                  {QStringLiteral("/var/log/audit/audit.log")},
                                  {Globals::LogLevelIds::INFORMATION_LOG_LEVEL_ID})
{
}
