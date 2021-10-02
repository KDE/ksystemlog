/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <QPainter>
#include <QRect>
class QPrinter;

class LogViewWidget;

class LogViewExport : public QObject
{
    Q_OBJECT

public:
    explicit LogViewExport(QWidget *parent, LogViewWidget *logViewWidget);

    ~LogViewExport() override;

    void copyToClipboard();

    void fileSave();

    void sendMail();

    void printSelection();

    void printPreview();
Q_SIGNALS:
    void statusBarChanged(const QString &message);

private:
    void printPageNumber(QPainter &painter, QRect &printView, int movement, int page, int margin);

    QWidget *const mParent;

    LogViewWidget *const mLogViewWidget;
    void print(QPrinter *printer);
};

