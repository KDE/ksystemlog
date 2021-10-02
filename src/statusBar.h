/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStatusBar>

class QString;
class QLabel;
class QComboBox;
class QPushButton;
namespace KSystemLog
{
/**
 * Status Bar
 */
class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent);

    ~StatusBar() override;

    void changeLineCountMessage(const QString &lineCountMessage);
    void changeLastModification(QTime lastModification);

    void changeMessage(const QString &message);

private Q_SLOTS:
    void toggleHistory();
    void selectLastHistory();

private:
    QLabel *mLineCountLabel = nullptr;
    QComboBox *mMessageList = nullptr;
    QLabel *mLastModificationLabel = nullptr;
    QPushButton *mToggleHistory = nullptr;
};
}

