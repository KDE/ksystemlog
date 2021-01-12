#ifndef _AUDIT_FACTORY_H_
#define _AUDIT_FACTORY_H_

#include <QList>

#include "logModeFactory.h"

class LogModeAction;
class LogMode;

class AuditLogModeFactory : public LogModeFactory
{
    Q_OBJECT

public:
    QList<LogMode *> createLogModes() const override;

    LogModeAction *createLogModeAction() const override;
};

#endif // _AUDIT_FACTORY_H_
