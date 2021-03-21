#pragma once

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

