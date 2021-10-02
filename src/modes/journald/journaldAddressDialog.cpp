/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com
    SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "journaldAddressDialog.h"

const quint16 JournaldDefaultPort = 19531;

JournaldAddressDialog::JournaldAddressDialog(QWidget *parent, const QString &title, const QString &address, const QString &port, bool httpsEnabled)
    : QDialog(parent)
{
    setupUi(this);
    setWindowTitle(title);
    addressLineEdit->setText(address);
    if (!port.isEmpty()) {
        portLineEdit->setText(port);
    }
    httpsCheckBox->setChecked(httpsEnabled);
}

QString JournaldAddressDialog::address() const
{
    return addressLineEdit->text();
}

QString JournaldAddressDialog::port() const
{
    return portLineEdit->text();
}

bool JournaldAddressDialog::httpsEnabled() const
{
    return httpsCheckBox->isChecked();
}

void JournaldAddressDialog::accept()
{
    if (addressLineEdit->text().isEmpty()) {
        // Do not allow to enter empty address.
        return;
    }
    if (portLineEdit->text().isEmpty()) {
        portLineEdit->setText(QString::number(JournaldDefaultPort));
    }
    QDialog::accept();
}
