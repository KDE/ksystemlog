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

#include "journaldFactory.h"
#include "journaldAnalyzer.h"
#include "journaldLogMode.h"
#include "journaldConfiguration.h"
#include "logMode.h"
#include "logging.h"
#include "multipleActions.h"

#include <KLocalizedString>

QList<LogMode *> JournaldModeFactory::createLogModes() const
{
    QList<LogMode *> logModes;
    logModes.append(new JournaldLogMode());
    return logModes;
}

LogModeAction *JournaldModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID));

    MultipleActions *multipleActions = new MultipleActions(
        QIcon::fromTheme(QLatin1String(JOURNALD_LOG_MODE_ID)), i18n("Journald"), logMode);

    ActionData actionData;
    actionData.first = logMode->id();

    JournaldAnalyzerOptions options;
    options.analyzerType = JournaldAnalyzerType::Local;

    actionData.second = QVariant::fromValue(options);

    KActionMenu *actionMenu = new KActionMenu(i18n("Local journal"), multipleActions);

    // Add "All messages" action.
    QAction *action = new QAction(i18n("All messages"), actionMenu);
    action->setData(QVariant::fromValue(actionData));
    actionMenu->addAction(action);

    // Add separator.
    action = new QAction(actionMenu);
    action->setSeparator(true);
    actionMenu->addAction(action);

    // Add filtering by systemd unit.
    KActionMenu *filterActionMenu = new KActionMenu(i18n("Filter by systemd unit"), actionMenu);
    QStringList units = JournaldAnalyzer::units();
    for (const QString &unit : units) {
        action = new QAction(unit, filterActionMenu);

        options.filter = QString("_SYSTEMD_UNIT=%1").arg(unit);
        actionData.second = QVariant::fromValue(options);
        action->setData(QVariant::fromValue(actionData));

        filterActionMenu->addAction(action);
    }
    actionMenu->addAction(filterActionMenu);

    // Add filtering by syslog identifier.
    filterActionMenu = new KActionMenu(i18n("Filter by syslog identifier"), actionMenu);
    QStringList syslogIDs = JournaldAnalyzer::syslogIdentifiers();
    for (const QString &id : syslogIDs) {
        action = new QAction(id, filterActionMenu);

        options.filter = QString("SYSLOG_IDENTIFIER=%1").arg(id);
        actionData.second = QVariant::fromValue(options);
        action->setData(QVariant::fromValue(actionData));

        filterActionMenu->addAction(action);
    }
    actionMenu->addAction(filterActionMenu);

    multipleActions->addInnerAction(actionMenu);

    options.analyzerType = JournaldAnalyzerType::Network;

    JournaldConfiguration *configuration = logMode->logModeConfiguration<JournaldConfiguration *>();
    auto remoteJournals = configuration->remoteJournals();
    for (const auto &addressInfo : remoteJournals) {
        actionMenu = new KActionMenu(QString("%1:%2").arg(addressInfo.address).arg(addressInfo.port),
                                     multipleActions);

        action = new QAction(i18n("Connect"), actionMenu);
        options.address = addressInfo.address;
        options.port = addressInfo.port;
        action->setData(QVariant::fromValue(actionData));
        actionMenu->addAction(action);

        multipleActions->addInnerAction(actionMenu);
    }

    return multipleActions;
}
