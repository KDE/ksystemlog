/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>

class LogViewWidget;

#include "ui_detailDialogBase.h"

class DetailDialog : public QDialog, public Ui::DetailDialogBase
{
    Q_OBJECT

public:
    explicit DetailDialog(QWidget *parent);

    ~DetailDialog() override;

public Q_SLOTS:
    void selectionChanged(LogViewWidget *logViewWidget);

private Q_SLOTS:
    void previousItem();
    void nextItem();

private:
    void updateDetails();

    /**
     * Method that replaces the bugged itemAbove() and itemBelow() methods
     */
    void moveToItem(int direction);

    LogViewWidget *mLogViewWidget = nullptr;
};

