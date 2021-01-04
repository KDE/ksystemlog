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

#ifndef JOURNALD_ADDRESS_DIALOG_H
#define JOURNALD_ADDRESS_DIALOG_H

#include "ui_journaldAddressDialog.h"

#include <QDialog>

class JournaldAddressDialog : public QDialog, public Ui::JournaldAddressDialog
{
    Q_OBJECT

public:
    explicit JournaldAddressDialog(QWidget *parent, const QString &title, const QString &address = QString(),
                          const QString &port = QString(), bool httpsEnabled = false);

    QString address() const;
    QString port() const;
    bool httpsEnabled() const;

public Q_SLOTS:
    void accept() override;
};

#endif // _JOURNALD_ADDRESS_DIALOG_H
