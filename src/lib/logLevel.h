/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QColor>
#include <QIcon>
#include <QObject>
#include <QString>
class LogLevel : QObject
{
public:
    explicit LogLevel(int id, const QString &name, const QString &icon, const QColor &color, QObject *parent = nullptr);

    ~LogLevel() override;

    int id() const;
    QString name() const;

    QColor color() const;

    QIcon icon() const;

private:
    int mId;
    QString mName;

    QColor mColor;

    QIcon mIcon;
};

