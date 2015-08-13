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

const quint16 JournaldDefaultPort = 19531;

JournaldAddressDialog::JournaldAddressDialog(QWidget *parent, QString title, QString address, QString port)
    : QDialog(parent)
{
    setupUi(this);
    setWindowTitle(title);
    addressLineEdit->setText(address);
    if (!port.isEmpty())
        portLineEdit->setText(port);
}

QString JournaldAddressDialog::address() const
{
    return addressLineEdit->text();
}

QString JournaldAddressDialog::port() const
{
    return portLineEdit->text();
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
