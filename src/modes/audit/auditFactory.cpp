#include "auditFactory.h"

#include <QList>

#include "logMode.h"
#include "logging.h"

#include "auditLogMode.h"
#include "simpleAction.h"

#include "logModeFactory.h"

QList<LogMode *> AuditLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new AuditLogMode()};
    return logModes;
}

LogModeAction *AuditLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(AUDIT_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    SimpleAction *logModeAction = new SimpleAction(logMode->action(), logMode);

    return logModeAction;
}
