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
