#include "auditConfigurationWidget.h"

#include <KLocalizedString>

#include "globals.h"

#include "logLevelFileList.h"
#include "logging.h"

#include "auditConfiguration.h"
#include "auditLogMode.h"

AuditConfigurationWidget::AuditConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Audit Log"), QStringLiteral(AUDIT_MODE_ICON), i18n("Audit Log"))
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    const QString description = i18n("<p>These files will be analyzed to show the <b>Audit logs</b>.</p>");

    fileList = new LogLevelFileList(this, description);

    connect(fileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(fileList);
}

bool AuditConfigurationWidget::isValid() const
{
    if (!fileList->isEmpty()) {
        logDebug() << "Audit configuration valid";
        return true;
    }

    logDebug() << "Audit configuration not valid";
    return false;
}

void AuditConfigurationWidget::saveConfig()
{
    logDebug() << "Saving config from Audit Options...";

    AuditConfiguration *auditConfiguration = Globals::instance().findLogMode(QStringLiteral(AUDIT_LOG_MODE_ID))->logModeConfiguration<AuditConfiguration *>();
    auditConfiguration->setLogFilesPaths(fileList->paths());
    auditConfiguration->setLogFilesLevels(fileList->levels());
}

void AuditConfigurationWidget::readConfig()
{
    AuditConfiguration *auditConfiguration = Globals::instance().findLogMode(QStringLiteral(AUDIT_LOG_MODE_ID))->logModeConfiguration<AuditConfiguration *>();
    fileList->removeAllItems();
    fileList->addPaths(auditConfiguration->logFilesPaths(), auditConfiguration->logFilesLevels());
}

void AuditConfigurationWidget::defaultConfig()
{
    readConfig();
}
