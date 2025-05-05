/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>
    SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "journaldConfigurationWidget.h"
#include "globals.h"
#include "journaldAddressDialog.h"
#include "journaldConfiguration.h"

#include <KLocalizedString>

#include <QCheckBox>

JournaldConfigurationWidget::JournaldConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Journald Log"), QLatin1String(JOURNALD_MODE_ICON), i18n("Journald Log"))
{
    setupUi(this);

    remoteJournalsListWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(remoteJournalsListWidget, &QTableWidget::itemSelectionChanged, this, &JournaldConfigurationWidget::updateButtons);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(lastBootOnly, &QCheckBox::stateChanged, this, &JournaldConfigurationWidget::configurationChanged);
#else
    connect(lastBootOnly, &QCheckBox::checkStateChanged, this, &JournaldConfigurationWidget::configurationChanged);
#endif
    connect(entriesTypeComboBox, &QComboBox::currentIndexChanged, this, &JournaldConfigurationWidget::configurationChanged);

    connect(addAddressButton, &QPushButton::clicked, this, &JournaldConfigurationWidget::addRemoteJournal);
    connect(modifyAddressButton, &QPushButton::clicked, this, &JournaldConfigurationWidget::modifyRemoteJournal);
    connect(removeAddressButton, &QPushButton::clicked, this, &JournaldConfigurationWidget::removeRemoteJournal);
    connect(remoteJournalsListWidget, &QTableWidget::cellDoubleClicked, this, &JournaldConfigurationWidget::tableItemClicked);
}

void JournaldConfigurationWidget::saveConfig()
{
    auto *configuration = Globals::instance().findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID))->logModeConfiguration<JournaldConfiguration *>();

    configuration->setDisplayCurrentBootOnly(lastBootOnly->isChecked());
    configuration->setEntriesType((JournaldConfiguration::EntriesType)entriesTypeComboBox->currentIndex());

    QVector<JournalAddress> remoteJournals;
    for (int row = 0; row < remoteJournalsListWidget->rowCount(); row++) {
        QTableWidgetItem *addressItem = remoteJournalsListWidget->item(row, 0);
        QTableWidgetItem *portItem = remoteJournalsListWidget->item(row, 1);
        QTableWidgetItem *httpsItem = remoteJournalsListWidget->item(row, 2);
        JournalAddress addressInfo;
        addressInfo.address = addressItem->text();
        addressInfo.port = portItem->text().toUInt();
        Qt::CheckState const httpsCheckState = httpsItem->checkState();
        addressInfo.https = (httpsCheckState == Qt::Checked);
        remoteJournals.append(addressInfo);
    }
    configuration->setRemoteJournals(remoteJournals);

    Q_EMIT configSaved();
}

void JournaldConfigurationWidget::readConfig()
{
    auto *configuration = Globals::instance().findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID))->logModeConfiguration<JournaldConfiguration *>();

    lastBootOnly->setChecked(configuration->displayCurrentBootOnly());
    entriesTypeComboBox->setCurrentIndex(configuration->entriesType());

    remoteJournalsListWidget->clearContents();
    while (remoteJournalsListWidget->rowCount() > 0) {
        remoteJournalsListWidget->removeRow(0);
    }

    const QVector<JournalAddress> remoteJournals = configuration->remoteJournals();
    for (const JournalAddress &addressInfo : remoteJournals) {
        if (haveJournalAddress(addressInfo.address, QString::number(addressInfo.port), addressInfo.https)) {
            continue;
        }
        remoteJournalsListWidget->insertRow(remoteJournalsListWidget->rowCount());
        remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 0, new QTableWidgetItem(addressInfo.address));
        remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(addressInfo.port)));
        auto item = new QTableWidgetItem(i18n("Enabled"));
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
    const auto selectedItems = remoteJournalsListWidget->selectedItems();
    const bool haveItems = !selectedItems.empty();
    modifyAddressButton->setEnabled(haveItems);
    removeAddressButton->setEnabled(haveItems);
}

void JournaldConfigurationWidget::addRemoteJournal()
{
    JournaldAddressDialog dialog(this, i18n("Add remote journal"));
    if (dialog.exec() == QDialog::Accepted) {
        const QString address = dialog.address();
        const QString port = dialog.port();
        const bool httpsEnabled = dialog.httpsEnabled();

        if (!haveJournalAddress(address, port, httpsEnabled)) {
            remoteJournalsListWidget->insertRow(remoteJournalsListWidget->rowCount());
            remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 0, new QTableWidgetItem(address));
            remoteJournalsListWidget->setItem(remoteJournalsListWidget->rowCount() - 1, 1, new QTableWidgetItem(port));
            auto item = new QTableWidgetItem(i18n("Enabled"));
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
    bool const httpsEnabled = (Qt::Checked == httpsItem->checkState());
    JournaldAddressDialog dialog(this, i18n("Modify remote journal"), addressItem->text(), portItem->text(), httpsEnabled);
    if (dialog.exec() == QDialog::Accepted) {
        QString const address = dialog.address();
        QString const port = dialog.port();
        bool const newHttpsEnabled = dialog.httpsEnabled();
        if (!haveJournalAddress(address, port, newHttpsEnabled)) {
            addressItem->setText(address);
            portItem->setText(port);
            httpsItem->setCheckState(newHttpsEnabled ? Qt::Checked : Qt::Unchecked);
        }
    }
}

bool JournaldConfigurationWidget::haveJournalAddress(const QString &address, const QString &port, bool httpsEnabled) const
{
    for (int row = 0; row < remoteJournalsListWidget->rowCount(); row++) {
        QTableWidgetItem *addressItem = remoteJournalsListWidget->item(row, 0);
        QTableWidgetItem *portItem = remoteJournalsListWidget->item(row, 1);
        QTableWidgetItem *httpsItem = remoteJournalsListWidget->item(row, 2);
        bool const https = (Qt::Checked == httpsItem->checkState());
        if ((addressItem->text() == address) && (portItem->text() == port) && (https == httpsEnabled)) {
            return true;
        }
    }
    return false;
}

#include "moc_journaldConfigurationWidget.cpp"
