/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "journaldFactory.h"
#include "journaldConfiguration.h"
#include "journaldLocalAnalyzer.h"
#include "journaldLogMode.h"
#include "logMode.h"
#include "logging.h"
#include "multipleActions.h"

#include <KLocalizedString>

QList<LogMode *> JournaldModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new JournaldLogMode()};
    return logModes;
}

LogModeAction *JournaldModeFactory::createLogModeAction() const
{
    JournaldLogMode *logMode = dynamic_cast<JournaldLogMode *>(Globals::instance().findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID)));

    auto multipleActions = new MultipleActions(QIcon::fromTheme(QLatin1String(JOURNALD_MODE_ICON)), i18n("Journald"), logMode);

    QIcon filterIcon = QIcon::fromTheme(QLatin1String("view-filter"));
    QIcon remoteIcon = QIcon::fromTheme(QLatin1String("preferences-system-network-sharing"));

    // Do not add journald submenu actions into action collection.
    // There are too many of them and submenu contents sometimes gets changed.
    ActionData actionData;
    actionData.id = logMode->id();
    actionData.addToActionCollection = false;

    JournaldAnalyzerOptions analyzerOptions;
    analyzerOptions.analyzerType = JournaldAnalyzerType::Local;

    actionData.analyzerOptions = QVariant::fromValue(analyzerOptions);

    auto actionMenu = new KActionMenu(QIcon::fromTheme(QLatin1String("drive-harddisk")), i18n("Local journal"), multipleActions);

    // Add "All messages" action.
    auto action = new QAction(filterIcon, i18n("All messages"), actionMenu);
    action->setData(QVariant::fromValue(actionData));
    actionMenu->addAction(action);
    multipleActions->addInnerAction(action, false, true);

    // Add separator.
    action = new QAction(actionMenu);
    action->setSeparator(true);
    actionMenu->addAction(action);

    // Add filtering by systemd unit.
    auto filterActionMenu = new KActionMenu(filterIcon, i18n("Filter by systemd unit"), actionMenu);
    const QStringList units = JournaldLocalAnalyzer::unitsStatic();
    for (const QString &unit : units) {
        action = new QAction(unit, filterActionMenu);

        analyzerOptions.filter = QStringLiteral("_SYSTEMD_UNIT=%1").arg(unit);
        actionData.analyzerOptions = QVariant::fromValue(analyzerOptions);
        action->setData(QVariant::fromValue(actionData));

        filterActionMenu->addAction(action);
        multipleActions->addInnerAction(action, false, true);
    }
    actionMenu->addAction(filterActionMenu);

    // Add filtering by syslog identifier.
    filterActionMenu = new KActionMenu(filterIcon, i18n("Filter by syslog identifier"), actionMenu);
    QStringList syslogIDs = JournaldLocalAnalyzer::syslogIdentifiersStatic();
    for (const QString &id : syslogIDs) {
        action = new QAction(id, filterActionMenu);

        analyzerOptions.filter = QStringLiteral("SYSLOG_IDENTIFIER=%1").arg(id);
        actionData.analyzerOptions = QVariant::fromValue(analyzerOptions);
        action->setData(QVariant::fromValue(actionData));

        filterActionMenu->addAction(action);
        multipleActions->addInnerAction(action, false, true);
    }
    actionMenu->addAction(filterActionMenu);

    multipleActions->addInnerAction(actionMenu, true, false);

    analyzerOptions.analyzerType = JournaldAnalyzerType::Network;
    analyzerOptions.filter.clear();

    action = new QAction(multipleActions);
    action->setSeparator(true);
    multipleActions->addInnerAction(action, true, false);

    // Create remote journal submenus.
    auto *configuration = logMode->logModeConfiguration<JournaldConfiguration *>();
    auto remoteJournals = configuration->remoteJournals();
    for (const auto &addressInfo : remoteJournals) {
        QString menuText = QStringLiteral("%1:%2").arg(addressInfo.address).arg(addressInfo.port);
        actionMenu = new KActionMenu(remoteIcon, menuText, multipleActions);

        action = new QAction(QIcon::fromTheme(QLatin1String("network-connect")), i18n("Connect"), actionMenu);
        analyzerOptions.address = addressInfo;
        actionData.analyzerOptions = QVariant::fromValue(analyzerOptions);
        action->setData(QVariant::fromValue(actionData));
        actionMenu->addAction(action);
        multipleActions->addInnerAction(action, false, true);

        // Add separator.
        action = new QAction(actionMenu);
        action->setSeparator(true);
        actionMenu->addAction(action);

        // Add filtering by systemd unit.
        JournalFilters filters = logMode->filters(addressInfo);
        if (!filters.systemdUnits.isEmpty()) {
            auto filterActionSystemdMenu = new KActionMenu(filterIcon, i18n("Filter by systemd unit"), actionMenu);

            for (const QString &unit : filters.systemdUnits) {
                action = new QAction(unit, filterActionSystemdMenu);

                analyzerOptions.filter = QStringLiteral("_SYSTEMD_UNIT=%1").arg(unit);
                actionData.analyzerOptions = QVariant::fromValue(analyzerOptions);
                action->setData(QVariant::fromValue(actionData));

                filterActionSystemdMenu->addAction(action);
                multipleActions->addInnerAction(action, false, true);
            }
            actionMenu->addAction(filterActionSystemdMenu);
        }

        // Add filtering by syslog identifier.
        if (!filters.syslogIdentifiers.isEmpty()) {
            auto filterActionMenu = new KActionMenu(filterIcon, i18n("Filter by syslog identifier"), actionMenu);

            for (const QString &id : filters.syslogIdentifiers) {
                action = new QAction(id, filterActionMenu);

                analyzerOptions.filter = QStringLiteral("SYSLOG_IDENTIFIER=%1").arg(id);
                actionData.analyzerOptions = QVariant::fromValue(analyzerOptions);
                action->setData(QVariant::fromValue(actionData));

                filterActionMenu->addAction(action);
                multipleActions->addInnerAction(action, false, true);
            }
            actionMenu->addAction(filterActionMenu);
        }

        multipleActions->addInnerAction(actionMenu, true, false);
    }

    // Add default log action with icon.
    // Don't put in into the menu, but allow it to be added into action collection and placed on the toolbar.
    multipleActions->addInnerAction(logMode->action(), false, true);

    return multipleActions;
}
