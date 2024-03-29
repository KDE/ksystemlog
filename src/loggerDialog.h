/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "ui_loggerDialogBase.h"

class LoggerDialog : public QDialog, public Ui::LoggerDialogBase
{
    Q_OBJECT
public:
    explicit LoggerDialog(QWidget *parent = nullptr);

    ~LoggerDialog() override;

    void initialize();

protected Q_SLOTS:
    void sendMessage();

    void textChanged();

    void changeTagActivation(bool activation);
    void changeFileActivation(bool activation);
    void changeMessageActivation(bool activation);

private:
    void slotLinkClicked(const QString &link);
    void buildMaps();
    QMap<QString, QString> mFacilities;

    QMap<QString, QString> mPriorities;
    QMap<QString, QIcon> mPriorityIcons;
};
