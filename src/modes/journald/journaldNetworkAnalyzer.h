/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>
    SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "journaldAnalyzer.h"
#include "journaldTypes.h"

using namespace JournaldTypes;

#include <QNetworkAccessManager>
#include <QNetworkReply>

class JournaldNetworkAnalyzer : public JournaldAnalyzer
{
    Q_OBJECT

public:
    explicit JournaldNetworkAnalyzer(LogMode *mode, const JournaldAnalyzerOptions &options);

    void watchLogFiles(bool enabled) override;

    QStringList units() const override;

    QStringList syslogIdentifiers() const override;

private Q_SLOTS:
    void httpFinished();
    void httpReadyRead();
    void httpError(QNetworkReply::NetworkError code);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    enum class RequestType { SyslogIds, Units, EntriesFull, EntriesUpdate };

    void parseEntries(QByteArray &data, ReadingMode readingMode);
    void sendRequest(RequestType requestType);

    void updateStatus(const QString &status);

    RequestType mCurrentRequest;
    QString mBaseUrl;
    QString mEntriesUrlFull;
    QString mEntriesUrlUpdating;
    QString mSyslogIdUrl;
    QString mSystemdUnitsUrl;

    QStringList mSyslogIdentifiers;
    QStringList mSystemdUnits;

    QNetworkAccessManager mNetworkManager;
    QNetworkReply *mReply = nullptr;
    QString mCursor;
    JournalAddress mAddress;
    QString mFilterName;
};

