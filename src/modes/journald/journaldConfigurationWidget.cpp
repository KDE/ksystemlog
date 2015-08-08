/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *   Copyright (C) 2015 by Vyacheslav Matyushin                            *
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

#include "journaldAddressDialog.h"
#include "journaldConfigurationWidget.h"
#include "journaldConfiguration.h"
#include "globals.h"

#include <KLocalizedString>

JournaldConfigurationWidget::JournaldConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Journald Log"), QLatin1String(JOURNALD_MODE_ICON),
                                 i18n("Journald Log"))
{
    setupUi(this);

    remoteJournalsListWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(remoteJournalsListWidget, SIGNAL(itemSelectionChanged()), SLOT(updateButtons()));

    connect(lastBootOnly, SIGNAL(stateChanged(int)), SIGNAL(configurationChanged()));
    connect(currentUserEntries, SIGNAL(stateChanged(int)), SIGNAL(configurationChanged()));
    connect(systemEntries, SIGNAL(stateChanged(int)), SIGNAL(configurationChanged()));

    connect(addAddressButton, SIGNAL(clicked(bool)), SLOT(addRemoteJournal()));
    connect(modifyAddressButton, SIGNAL(clicked(bool)), SLOT(modifyRemoteJournal()));
    connect(removeAddressButton, SIGNAL(clicked(bool)), SLOT(removeRemoteJournal()));
    connect(remoteJournalsListWidget, SIGNAL(cellDoubleClicked(int, int)), SLOT(tableItemClicked(int)));
}

void JournaldConfigurationWidget::saveConfig()
{
    JournaldConfiguration *configuration = Globals::instance()
                                               .findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID))
                                               ->logModeConfiguration<JournaldConfiguration *>();

    configuration->setDisplayCurrentBootOnly(lastBootOnly->isChecked());
    configuration->setDisplayCurrentUserProcesses(currentUserEntries->isChecked());
    configuration->setDisplaySystemServices(systemEntries->isChecked());

    QList<JournaldConfiguration::RemoteJournalAddress> remoteJournals;
    for (int row = 0; row < remoteJournalsListWidget->rowCount(); row++) {
        QTableWidgetItem *addressItem = remoteJournalsListWidget->item(row, 0);
        QTableWidgetItem *portItem = remoteJournalsListWidget->item(row, 1);
        JournaldConfiguration::RemoteJournalAddress addressInfo;
        addressInfo.address = addressItem->text();
        addressInfo.port = portItem->text().toUInt();
        remoteJournals.append(addressInfo);
    }
    configuration->setRemoteJournals(remoteJournals);
}

void JournaldConfigurationWidget::readConfig()
{
    JournaldConfiguration *configuration = Globals::instance()
                                               .findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID))
                                               ->logModeConfiguration<JournaldConfiguration *>();

    lastBootOnly->setChecked(configuration->displayCurrentBootOnly());
    currentUserEntries->setChecked(configuration->displayCurrentUserProcesses());
    systemEntries->setChecked(configuration->displaySystemServices());

    remoteJournalsListWidget->clearContents();
    QList<JournaldConfiguration::RemoteJournalAddress> remoteJournals = configuration->remoteJournals();
    for (const JournaldConfiguration::RemoteJournalAddress &addressInfo : remoteJournals) {
        remoteJournalsListWidget->insertRow(remoteJournalsListWidget->rowCount());
        remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 0,
                                          new QTableWidgetItem(addressInfo.address));
        remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 1,
                                          new QTableWidgetItem(QString::number(addressInfo.port)));
    }
}

void JournaldConfigurationWidget::defaultConfig()
{
    readConfig();
}

void JournaldConfigurationWidget::updateButtons()
{
    auto selectedItems = remoteJournalsListWidget->selectedItems();
    bool haveItems = (selectedItems.size() != 0);
    modifyAddressButton->setEnabled(haveItems);
    removeAddressButton->setEnabled(haveItems);
}

void JournaldConfigurationWidget::addRemoteJournal()
{
    JournaldAddressDialog dialog(i18n("Add remote journal"));
    if (dialog.exec() == QDialog::Accepted) {
        QString address = dialog.address();
        QString port = dialog.port();

        if (!haveJournalAddress(address, port)) {
            remoteJournalsListWidget->insertRow(remoteJournalsListWidget->rowCount());
            remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 0,
                                              new QTableWidgetItem(address));
            remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 1,
                                              new QTableWidgetItem(port));
        }
    }
}

void JournaldConfigurationWidget::modifyRemoteJournal()
{
    tableItemClicked(remoteJournalsListWidget->currentRow());
}

void JournaldConfigurationWidget::removeRemoteJournal()
{
    remoteJournalsListWidget->removeRow(remoteJournalsListWidget->currentRow());
}

void JournaldConfigurationWidget::tableItemClicked(int row)
{
    QTableWidgetItem *addressItem = remoteJournalsListWidget->item(row, 0);
    QTableWidgetItem *portItem = remoteJournalsListWidget->item(row, 1);
    JournaldAddressDialog dialog(i18n("Modify remote journal"), addressItem->text(), portItem->text());
    if (dialog.exec() == QDialog::Accepted) {
        QString address = dialog.address();
        QString port = dialog.port();
        if (!haveJournalAddress(address, port)) {
            addressItem->setText(address);
            portItem->setText(port);
        }
    }
}

bool JournaldConfigurationWidget::haveJournalAddress(QString address, QString port) const
{
    for (int row = 0; row < remoteJournalsListWidget->rowCount(); row++) {
        QTableWidgetItem *addressItem = remoteJournalsListWidget->item(row, 0);
        QTableWidgetItem *portItem = remoteJournalsListWidget->item(row, 1);
        if ((addressItem->text() == address) && (portItem->text() == port))
            return true;
    }
    return false;
}
