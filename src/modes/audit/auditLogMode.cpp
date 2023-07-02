#include "auditLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "auditAnalyzer.h"
#include "auditConfiguration.h"
#include "auditConfigurationWidget.h"

#include "auditItemBuilder.h"

AuditLogMode::AuditLogMode()
    : LogMode(QStringLiteral(AUDIT_LOG_MODE_ID), i18n("Audit Log"), QStringLiteral(AUDIT_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<AuditConfiguration>(new AuditConfiguration());

    d->logModeConfigurationWidget = new AuditConfigurationWidget();

    d->itemBuilder = new AuditItemBuilder();

    d->action = createDefaultAction();
    d->action->setText(i18n("Audit Log"));
    d->action->setToolTip(i18n("Display the audit log."));
    d->action->setWhatsThis(
        i18n("Displays the audit log in the current tab. This log is generally used by non-specialized processes "
             "(like \"sudo\" or \"fsck\" commands)"));

    checkLogFilesPresence(logModeConfiguration<AuditConfiguration *>()->logFilesPaths());
}

Analyzer *AuditLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new AuditAnalyzer(this);
}

QVector<LogFile> AuditLogMode::createLogFiles()
{
    return logModeConfiguration<AuditConfiguration *>()->findGenericLogFiles();
}

#include "moc_auditLogMode.cpp"
