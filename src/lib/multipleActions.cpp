/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "multipleActions.h"

#include <QAction>

#include "ksystemlog_debug.h"

MultipleActions::MultipleActions(const QIcon &icon, const QString &text, QObject *parent)
{
    mAction = new KActionMenu(icon, text, parent);
}

MultipleActions::~MultipleActions()
{
    delete mAction;
}

QList<QAction *> MultipleActions::innerActions()
{
    return mActions;
}

QAction *MultipleActions::actionMenu()
{
    return mAction;
}

void MultipleActions::addInnerAction(QAction *innerAction, bool addToMenu, bool addToInnerActionsList)
{
    if (addToMenu) {
        mAction->addAction(innerAction);
    }

    if (addToInnerActionsList) {
        mActions.append(innerAction);
    }
}

#include "moc_multipleActions.cpp"
