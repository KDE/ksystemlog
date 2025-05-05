/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>
    SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ui_journaldAddressDialog.h"

#include <QDialog>

class JournaldAddressDialog : public QDialog, public Ui::JournaldAddressDialog
{
    Q_OBJECT

public:
    explicit JournaldAddressDialog(QWidget *parent,
                                   const QString &title,
                                   const QString &address = QString(),
                                   const QString &port = QString(),
                                   bool httpsEnabled = false);

    [[nodiscard]] QString address() const;
    [[nodiscard]] QString port() const;
    [[nodiscard]] bool httpsEnabled() const;

public Q_SLOTS:
    void accept() override;
};
