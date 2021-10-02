/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "ui_logLevelSelectionDialogBase.h"

class LogLevelSelectionDialog : public QDialog, public Ui::LogLevelSelectionDialogBase
{
public:
    explicit LogLevelSelectionDialog(QWidget *parent = nullptr);

    ~LogLevelSelectionDialog() override;

    QListWidget *logLevels() const;
};

