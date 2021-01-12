#ifndef _AUDIT_CONFIGURATION_WIDGET_H_
#define _AUDIT_CONFIGURATION_WIDGET_H_

#include "logModeConfigurationWidget.h"

class QLineEdit;
class LogLevelFileList;

class AuditConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    AuditConfigurationWidget();

    bool isValid() const override;
    void saveConfig() override;
    void readConfig() override;
    void defaultConfig() override;

private:
    LogLevelFileList *fileList;
    QLineEdit *keyLineEdit;
};

#endif // _AUDIT_CONFIGURATION_WIDGET_H_
