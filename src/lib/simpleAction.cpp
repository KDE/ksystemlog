/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "simpleAction.h"

#include <QAction>

#include "logging.h"

SimpleAction::SimpleAction(const QIcon &icon, const QString &text, QObject *parent)
{
    mAction = new QAction(icon, text, parent);
}

SimpleAction::SimpleAction(QAction *originAction, QObject *parent)
{
    mAction = new QAction(parent);
    mAction->setIcon(originAction->icon());
    mAction->setText(originAction->text());
    mAction->setData(originAction->data());
    mAction->setToolTip(originAction->toolTip());
    mAction->setWhatsThis(originAction->whatsThis());
}

SimpleAction::~SimpleAction()
{
    delete mAction;
}

QList<QAction *> SimpleAction::innerActions()
{
    const QList<QAction *> actions{mAction};

    return actions;
}

QAction *SimpleAction::actionMenu()
{
    return mAction;
}
