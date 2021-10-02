/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "kernelLogMode.h"

#include <KLocalizedString>

#include "logging.h"

#include "kernelAnalyzer.h"
#include "kernelItemBuilder.h"

#include <QAction>

KernelLogMode::KernelLogMode()
    : LogMode(QStringLiteral(KERNEL_LOG_MODE_ID), i18n("Kernel Log"), QStringLiteral(KERNEL_MODE_ICON))
{
    d->logModeConfigurationWidget = nullptr;

    d->itemBuilder = new KernelItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the kernel log."));
    d->action->setWhatsThis(
        i18n("Displays the kernel log in the current tab. This log is only useful for users who want to know why "
             "the Kernel does not detect their hardware or what is the cause of the last <i>kernel "
             "panic/oops</i>."));
}

KernelLogMode::~KernelLogMode()
{
}

Analyzer *KernelLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new KernelAnalyzer(this);
}

QVector<LogFile> KernelLogMode::createLogFiles()
{
    const QVector<LogFile> logFiles{LogFile(QUrl::fromLocalFile(QStringLiteral("/bin/dmesg")), Globals::instance().informationLogLevel())};
    return logFiles;
}
