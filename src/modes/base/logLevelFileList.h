/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include "fileList.h"

class QPushButton;
class LogLevel;

class LogLevelFileList : public FileList
{
    Q_OBJECT

public:
    explicit LogLevelFileList(QWidget *parent, const QString &description);

    ~LogLevelFileList() override;

    QList<int> levels() const;

    void addPaths(const QStringList &filePaths, const QList<int> &fileLevels);

private:
    LogLevel *level(int i) const;

    void insertItem(LogLevel *level, const QString &itemText, bool missing = false);

    static int LogLevelRole;

    QPushButton *changeItem;

protected Q_SLOTS:
    void addItem() override;

private Q_SLOTS:
    void updateSpecificButtons();

    void changeItemType();
};

