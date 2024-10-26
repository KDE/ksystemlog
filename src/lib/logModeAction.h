/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QAction>
#include <QList>
#include <QObject>

class LogModeAction : public QObject
{
    Q_OBJECT

public:
    enum Category {
        RootCategory,
        ServicesCategory,
        OthersCategory
    };

    Q_DECLARE_FLAGS(Categories, Category)

    explicit LogModeAction(QObject *parent = nullptr);

    ~LogModeAction() override;

    virtual QList<QAction *> innerActions() = 0;

    virtual QAction *actionMenu() = 0;

    void setCategory(Category category);

    Category category() const;

private:
    LogModeAction::Category mCategory = LogModeAction::RootCategory;
    bool mInToolBar = true;
};
