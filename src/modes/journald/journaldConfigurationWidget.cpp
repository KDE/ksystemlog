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

#include <QCheckBox>

JournaldConfigurationWidget::JournaldConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Journald Log"), QLatin1String(JOURNALD_MODE_ICON),
                                 i18n("Journald Log"))
{
    setupUi(this);

    remoteJournalsListWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(remoteJournalsListWidget, &QTableWidget::itemSelectionChanged, this,
            &JournaldConfigurationWidget::updateButtons);

    connect(lastBootOnly, &QCheckBox::stateChanged, this, &JournaldConfigurationWidget::configurationChanged);
    connect(currentUserEntries, &QCheckBox::stateChanged, this,
            &JournaldConfigurationWidget::configurationChanged);
    connect(systemEntries, &QCheckBox::stateChanged, this,
            &JournaldConfigurationWidget::configurationChanged);

    connect(addAddressButton, &QPushButton::clicked, this, &JournaldConfigurationWidget::addRemoteJournal);
    connect(modifyAddressButton, &QPushButton::clicked, this,
            &JournaldConfigurationWidget::modifyRemoteJournal);
    connect(removeAddressButton, &QPushButton::clicked, this,
            &JournaldConfigurationWidget::removeRemoteJournal);
    connect(remoteJournalsListWidget, &QTableWidget::cellDoubleClicked, this,
            &JournaldConfigurationWidget::tableItemClicked);
}

void JournaldConfigurationWidget::saveConfig()
{
    JournaldConfiguration *configuration = Globals::instance()
                                               .findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID))
                                               ->logModeConfiguration<JournaldConfiguration *>();

    configuration->setDisplayCurrentBootOnly(lastBootOnly->isChecked());
    configuration->setDisplayCurrentUserProcesses(currentUserEntries->isChecked());
    configuration->setDisplaySystemServices(systemEntries->isChecked());

    QList<JournalAddress> remoteJournals;
    for (int row = 0; row < remoteJournalsListWidget->rowCount(); row++) {
        QTableWidgetItem *addressItem = remoteJournalsListWidget->item(row, 0);
        QTableWidgetItem *portItem = remoteJournalsListWidget->item(row, 1);
        QTableWidgetItem *httpsItem = remoteJournalsListWidget->item(row, 2);
        JournalAddress addressInfo;
        addressInfo.address = addressItem->text();
        addressInfo.port = portItem->text().toUInt();
        Qt::CheckState httpsCheckState = httpsItem->checkState();
        addressInfo.https = (httpsCheckState == Qt::Checked);
        remoteJournals.append(addressInfo);
    }
    configuration->setRemoteJournals(remoteJournals);

    emit configSaved();
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
    while (remoteJournalsListWidget->rowCount() > 0) {
        remoteJournalsListWidget->removeRow(0);
    }

    QList<JournalAddress> remoteJournals = configuration->remoteJournals();
    for (const JournalAddress &addressInfo : remoteJournals) {
        if (haveJournalAddress(addressInfo.address, QString::number(addressInfo.port), addressInfo.https))
            continue;
        remoteJournalsListWidget->insertRow(remoteJournalsListWidget->rowCount());
        remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 0,
                                          new QTableWidgetItem(addressInfo.address));
        remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 1,
                                          new QTableWidgetItem(QString::number(addressInfo.port)));
        QTableWidgetItem *item = new QTableWidgetItem(i18n("Enabled"));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(addressInfo.https ? Qt::Checked : Qt::Unchecked);
        remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 2, item);
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
    JournaldAddressDialog dialog(this, i18n("Add remote journal"));
    if (dialog.exec() == QDialog::Accepted) {
        QString address = dialog.address();
        QString port = dialog.port();
        bool httpsEnabled = dialog.httpsEnabled();

        if (!haveJournalAddress(address, port, httpsEnabled)) {
            remoteJournalsListWidget->insertRow(remoteJournalsListWidget->rowCount());
            remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 0,
                                              new QTableWidgetItem(address));
            remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 1,
                                              new QTableWidgetItem(port));
            QTableWidgetItem *item = new QTableWidgetItem(i18n("Enabled"));
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(dialog.httpsEnabled() ? Qt::Checked : Qt::Unchecked);
            remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 2, item);
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
    QTableWidgetItem *httpsItem = remoteJournalsListWidget->item(row, 2);
    bool httpsEnabled = (Qt::Checked == httpsItem->checkState());
    JournaldAddressDialog dialog(this, i18n("Modify remote journal"), addressItem->text(), portItem->text(),
                                 httpsEnabled);
    if (dialog.exec() == QDialog::Accepted) {
        QString address = dialog.address();
        QString port = dialog.port();
        bool newHttpsEnabled = dialog.httpsEnabled();
        if (!haveJournalAddress(address, port, newHttpsEnabled)) {
            addressItem->setText(address);
            portItem->setText(port);
            httpsItem->setCheckState(newHttpsEnabled ? Qt::Checked : Qt::Unchecked);
        }
    }
}

bool JournaldConfigurationWidget::haveJournalAddress(QString address, QString port, bool httpsEnabled) const
{
    for (int row = 0; row < remoteJournalsListWidget->rowCount(); row++) {
        QTableWidgetItem *addressItem = remoteJournalsListWidget->item(row, 0);
        QTableWidgetItem *portItem = remoteJournalsListWidget->item(row, 1);
        QTableWidgetItem *httpsItem = remoteJournalsListWidget->item(row, 2);
        bool https = (Qt::Checked == httpsItem->checkState());
        if ((addressItem->text() == address) && (portItem->text() == port) && (https == httpsEnabled))
            return true;
    }
    return false;
}
