/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "xsessionLogMode.h"

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "xsessionAnalyzer.h"
#include "xsessionConfiguration.h"
#include "xsessionConfigurationWidget.h"
#include "xsessionItemBuilder.h"

XSessionLogMode::XSessionLogMode()
    : LogMode(QStringLiteral(X_SESSION_LOG_MODE_ID), i18n("X Session Log"), QStringLiteral(X_SESSION_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<XSessionConfiguration>(new XSessionConfiguration());

    d->logModeConfigurationWidget = new XSessionConfigurationWidget();

    d->itemBuilder = new XSessionItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the X Session log."));
    d->action->setWhatsThis(
        i18n("Displays the X Session log in the current tab. X Session log is the place where graphical programs "
             "write their output. See this log if you want to know why a program has crashed, or why your desktop "
             "environment (Plasma, Gnome, Unity, Xfce, LXQt, ...) has not started."));

    auto *configuration = logModeConfiguration<XSessionConfiguration *>();
    checkLogFilesPresence(QStringList() << configuration->xsessionPath());
}

XSessionLogMode::~XSessionLogMode()
{
}

Analyzer *XSessionLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new XSessionAnalyzer(this);
}

QVector<LogFile> XSessionLogMode::createLogFiles()
{
    auto *configuration = Globals::instance().findLogMode(QStringLiteral(X_SESSION_LOG_MODE_ID))->logModeConfiguration<XSessionConfiguration *>();

    const QVector<LogFile> logFiles{configuration->findGenericLogFile(configuration->xsessionPath())};
    return logFiles;
}
