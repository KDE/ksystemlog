/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "postfixLogMode.h"

#include <KLocalizedString>

#include "logging.h"

#include "postfixAnalyzer.h"
#include "postfixConfiguration.h"
#include "postfixConfigurationWidget.h"

#include "logModeItemBuilder.h"

PostfixLogMode::PostfixLogMode()
    : LogMode(QStringLiteral(POSTFIX_LOG_MODE_ID), i18n("Postfix Log"), QStringLiteral(POSTFIX_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<PostfixConfiguration>(new PostfixConfiguration());

    d->logModeConfigurationWidget = new PostfixConfigurationWidget();

    d->itemBuilder = new LogModeItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Postfix log."));
    d->action->setWhatsThis(
        i18n("Displays the Postfix log in the current tab. Postfix is the most known and used mail server in the "
             "Linux world."));

    checkLogFilesPresence(logModeConfiguration<PostfixConfiguration *>()->logFilesPaths());
}

PostfixLogMode::~PostfixLogMode()
{
}

Analyzer *PostfixLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new PostfixAnalyzer(this);
}

QVector<LogFile> PostfixLogMode::createLogFiles()
{
    return logModeConfiguration<PostfixConfiguration *>()->findGenericLogFiles();
}
