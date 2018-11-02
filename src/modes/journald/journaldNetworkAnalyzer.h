/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *   Copyright (C) 2015 by Vyacheslav Matyushin                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#ifndef _JOURNALD_NETWORK_ANALYZER_H_
#define _JOURNALD_NETWORK_ANALYZER_H_

#include "journaldAnalyzer.h"
#include "journaldTypes.h"

using namespace JournaldTypes;

#include <QNetworkAccessManager>
#include <QNetworkReply>

class JournaldNetworkAnalyzer : public JournaldAnalyzer
{
    Q_OBJECT

public:
    explicit JournaldNetworkAnalyzer(LogMode *logMode, const JournaldAnalyzerOptions &options);

    virtual ~JournaldNetworkAnalyzer();

    virtual void watchLogFiles(bool enabled);

    virtual QStringList units() const;

    virtual QStringList syslogIdentifiers() const;

private Q_SLOTS:
    void httpFinished();
    void httpReadyRead();
    void httpError(QNetworkReply::NetworkError code);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    enum class RequestType { SyslogIds, Units, EntriesFull, EntriesUpdate };

    void parseEntries(QByteArray &data, ReadingMode readingMode);
    void sendRequest(RequestType requestType);

    void updateStatus(QString status);

    RequestType m_currentRequest;
    QString m_baseUrl;
    QString m_entriesUrlFull;
    QString m_entriesUrlUpdating;
    QString m_syslogIdUrl;
    QString m_systemdUnitsUrl;

    QStringList m_syslogIdentifiers;
    QStringList m_systemdUnits;

    QNetworkAccessManager m_networkManager;
    QNetworkReply *m_reply;
    QString m_cursor;
    JournalAddress m_address;
    QString m_filterName;
};

#endif // _JOURNALD_NETWORK_ANALYZER_H_
