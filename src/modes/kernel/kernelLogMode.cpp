/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
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
