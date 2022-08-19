/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QIcon>
#include <QUrl>
#include <QWidget>

class QAction;
class QPushButton;

/**
 * Class which maintains common behavior between
 * FileList classes
 */
class FileListHelper : public QObject
{
    Q_OBJECT

public:
    explicit FileListHelper(QWidget *parent);
    ~FileListHelper() override;

    QStringList findPaths(const QList<QUrl> &urls);
    bool isValidFile(const QUrl &url);
    QStringList expandJoker(const QUrl &url);

    QList<QUrl> openUrls() const;
    QUrl openUrl(const QString &originPath) const;

    /**
     * Change the enabled status of the button and of its QActions
     */
    void setEnabledAction(QPushButton *button, bool enabled);

    /**
     * TODO Do this inline (and remove this method)
     */
    void prepareButton(QPushButton *button, const QIcon &icon, const QObject *receiver, const char *member, QWidget *fileList);

    QAction *prepareButtonAndAction(QPushButton *button, const QIcon &icon);
    QAction *prepareButtonAndAction(QPushButton *button, const QIcon &icon, const QObject *receiver, const char *member);

private:
    QWidget *const mParent = nullptr;
};
