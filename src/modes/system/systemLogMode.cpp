/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "systemLogMode.h"

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "systemAnalyzer.h"
#include "systemConfiguration.h"
#include "systemConfigurationWidget.h"

#include "logModeItemBuilder.h"

SystemLogMode::SystemLogMode()
    : LogMode(QStringLiteral(SYSTEM_LOG_MODE_ID), i18n("System Log"), QStringLiteral(SYSTEM_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<SystemConfiguration>(new SystemConfiguration());

    d->logModeConfigurationWidget = new SystemConfigurationWidget();

    d->itemBuilder = new LogModeItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the system log."));
    d->action->setWhatsThis(
        i18n("Displays the system log in the current tab. This log is generally used by non-specialized processes "
             "(like \"sudo\" or \"fsck\" commands)"));

    checkLogFilesPresence(logModeConfiguration<SystemConfiguration *>()->logFilesPaths());
}

SystemLogMode::~SystemLogMode()
{
}

Analyzer *SystemLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new SystemAnalyzer(this);
}

QVector<LogFile> SystemLogMode::createLogFiles()
{
    return logModeConfiguration<SystemConfiguration *>()->findGenericLogFiles();
}

#include "moc_systemLogMode.cpp"
