/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QIcon>
#include <QList>
#include <QString>

#include "globals.h"

#include "logModeAction.h"

class QAction;

class SimpleAction : public LogModeAction
{
    Q_OBJECT

public:
    SimpleAction(const QIcon &icon, const QString &text, QObject *parent);

    SimpleAction(QAction *action, QObject *parent);

    ~SimpleAction() override;

    QList<QAction *> innerActions() override;

    QAction *actionMenu() override;

private:
    QAction *mAction = nullptr;
};

