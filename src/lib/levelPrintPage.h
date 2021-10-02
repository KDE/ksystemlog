/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include <QString>
#include <QWidget>

class QButtonGroup;
class QVBoxLayout;
class QGridLayout;
class QCheckBox;
class QLabel;

/**
 @author Bojan Djurkovic
 */
class LevelPrintPage : public QWidget
{
public:
    explicit LevelPrintPage(QWidget *parent = nullptr);
    ~LevelPrintPage() override;

    bool isValid(QString &msg);

private:
    QButtonGroup *mBtnGroup = nullptr;
    QGridLayout *mBtnGroupLayout = nullptr;
    QVBoxLayout *mPageLayout = nullptr;
    QLabel *mLblChoose = nullptr;
    QList<QCheckBox *> mLevelCheckBoxes;
};

