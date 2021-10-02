/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QList>
#include <QObject>
#include <QString>

#include <KActionMenu>

#include "globals.h"

#include "logModeAction.h"
#include <QIcon>
class QAction;

/**
 * This class is used by LogModeFactory to provide multiple actions
 * and be able to retrieve them when necessary, using actions().
 * This method will return QAction added using addInnerAction().
 */
class MultipleActions : public LogModeAction
{
    Q_OBJECT

public:
    MultipleActions(const QIcon &icon, const QString &text, QObject *parent);

    ~MultipleActions() override;

    QList<QAction *> innerActions() override;

    QAction *actionMenu() override;

    /**
     * This method is not called addAction() to avoid name collision with
     * parent class
     */
    void addInnerAction(QAction *action, bool addToMenu = true, bool addToInnerActionsList = true);

private:
    QList<QAction *> mActions;

    KActionMenu *mAction = nullptr;
};

