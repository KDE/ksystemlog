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

#include <QList>
#include <QAction>
#include <KLocalizedString>

#include "logging.h"

#include "logMode.h"

#include "kernelAnalyzer.h"
#include "kernelItemBuilder.h"

KernelLogMode::KernelLogMode()
    : LogMode(QLatin1String(KERNEL_LOG_MODE_ID), i18n("Kernel Log"), QLatin1String(KERNEL_MODE_ICON))
{
    d->logModeConfiguration = NULL;

    d->logModeConfigurationWidget = NULL;

    d->itemBuilder = new KernelItemBuilder();

    d->analyzer = new KernelAnalyzer(this);

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the kernel log."));
    d->action->setWhatsThis(i18n(
        "Displays the kernel log in the current tab. This log is only useful for users who want to know why "
        "the Kernel does not detect their hardware or what is the cause of the last <i>kernel "
        "panic/oops</i>."));
}

KernelLogMode::~KernelLogMode()
{
}

QList<LogFile> KernelLogMode::createLogFiles()
{
    QList<LogFile> logFiles;
    logFiles.append(LogFile(QUrl::fromLocalFile("/bin/dmesg"), Globals::instance().informationLogLevel()));
    return logFiles;
}
