/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "xorgLogMode.h"

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "xorgAnalyzer.h"
#include "xorgConfiguration.h"
#include "xorgConfigurationWidget.h"
#include "xorgItemBuilder.h"

XorgLogMode::XorgLogMode()
    : LogMode(QStringLiteral(XORG_LOG_MODE_ID), i18n("X.org Log"), QStringLiteral(XORG_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<XorgConfiguration>(new XorgConfiguration());

    d->logModeConfigurationWidget = new XorgConfigurationWidget();

    d->itemBuilder = new XorgItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the X.org log."));
    d->action->setWhatsThis(
        i18n("Displays the X.org log in the current tab. X.org is the service which displays on screen your "
             "desktop and manage your graphical hardware. See this log if you want to know why you do not have 3D "
             "accelerations or why your input device is not recognized."));

    auto *configuration = logModeConfiguration<XorgConfiguration *>();
    checkLogFilesPresence(configuration->xorgPaths());
}

XorgLogMode::~XorgLogMode()
{
}

Analyzer *XorgLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new XorgAnalyzer(this);
}

QVector<LogFile> XorgLogMode::createLogFiles()
{
    auto *configuration = logModeConfiguration<XorgConfiguration *>();
    return configuration->findNoModeLogFiles(configuration->xorgPaths());
}

#include "moc_xorgLogMode.cpp"
