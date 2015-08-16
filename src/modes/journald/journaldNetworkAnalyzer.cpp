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

#include "journaldNetworkAnalyzer.h"
#include "journaldConfiguration.h"
#include "ksystemlogConfig.h"
#include "logging.h"
#include "logViewModel.h"
#include "logFile.h"

#include <KLocalizedString>

JournaldNetworkAnalyzer::JournaldNetworkAnalyzer(LogMode *logMode, QString address, quint16 port,
                                                 QString filter)
    : JournaldAnalyzer(logMode)
{
    // TODO: add support for HTTPS. Process sslErrors().
    JournaldConfiguration *configuration = logMode->logModeConfiguration<JournaldConfiguration *>();

    QString urlMain = QString("http://%1:%2/").arg(address).arg(port);

    m_entriesUrl = urlMain + "entries?";
    if (configuration->displayCurrentBootOnly())
        m_entriesUrl.append("boot&");
    m_entriesUrl.append("follow");
    if (!filter.isEmpty())
        m_entriesUrl.append("&" + filter);

    m_syslogIdUrl = urlMain + "fields/SYSLOG_IDENTIFIER";
    m_systemdUnitsUrl = urlMain + "fields/_SYSTEMD_UNIT";

    m_reply = nullptr;
}

JournaldNetworkAnalyzer::~JournaldNetworkAnalyzer()
{
}

void JournaldNetworkAnalyzer::watchLogFiles(bool enabled)
{
    if (enabled) {
        m_data.clear();
        sendRequest(RequestType::SyslogIds);
    } else {
        if (m_reply) {
            m_reply->abort();
            m_reply->deleteLater();
            m_reply = nullptr;
        }
    }
}

QStringList JournaldNetworkAnalyzer::units()
{
    return m_systemdUnits;
}

QStringList JournaldNetworkAnalyzer::syslogIdentifiers()
{
    return m_syslogIdentifiers;
}

void JournaldNetworkAnalyzer::httpFinished()
{
    QByteArray data = m_reply->readAll();
    if (m_currentRequest == RequestType::EntriesFull) {
        parseEntries(data, FullRead);
        if (!m_cursor.isEmpty())
            sendRequest(RequestType::EntriesUpdate);
        else {
            logWarning() << "Network journal analyzer failed to extract cursor string. "
                            "Updates will be unavailable";
        }
    } else {
        QString identifiersString = QString::fromUtf8(data);
        QStringList identifiersList = identifiersString.split('\n', QString::SkipEmptyParts);
        switch (m_currentRequest) {
        case RequestType::SyslogIds:
            m_syslogIdentifiers = identifiersList;
            m_syslogIdentifiers.sort();
            logDebug() << "Got syslogs:";
            logDebug() << m_syslogIdentifiers;
            sendRequest(RequestType::Units);
            break;
        case RequestType::Units:
            m_systemdUnits = identifiersList;
            m_systemdUnits.sort();
            logDebug() << "Got units:";
            logDebug() << m_systemdUnits;
            sendRequest(RequestType::EntriesFull);
            break;
        default:
            break;
        }
    }
    logDebug() << "Finished";
}

void JournaldNetworkAnalyzer::httpReadyRead()
{
    logDebug() << m_reply->bytesAvailable();
    if (m_currentRequest == RequestType::EntriesUpdate) {
        QByteArray data = m_reply->readAll();
        parseEntries(data, UpdatingRead);
    }
}

void JournaldNetworkAnalyzer::error(QNetworkReply::NetworkError code)
{
    // TODO: handle errors
    logWarning() << "Network error:" << code;
}

void JournaldNetworkAnalyzer::parseEntries(QByteArray &data, Analyzer::ReadingMode readingMode)
{
    QList<QByteArray> items = data.split('{');
    for (int i = 0; i < items.size(); i++) {
        // TODO: skip the last item. Receive it in update request.
        QByteArray &item = items[i];
        if (item.isEmpty())
            continue;
        item.prepend('{');
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(item, &jsonError);
        if (jsonError.error != 0)
            continue;
        QJsonObject object = doc.object();
        if (i == items.size() - 1)
            m_cursor = object["__CURSOR"].toString();
        //            logDebug() << object["MESSAGE"].toString();
    }
}

void JournaldNetworkAnalyzer::sendRequest(RequestType requestType)
{
    if (m_reply)
        m_reply->deleteLater();

    m_currentRequest = requestType;

    QNetworkRequest request;
    QString url;

    switch (requestType) {
    case RequestType::SyslogIds:
        url = m_syslogIdUrl;
        break;
    case RequestType::Units:
        url = m_systemdUnitsUrl;
        break;
    case RequestType::EntriesFull: {
        url = m_entriesUrl;
        int entries = KSystemLogConfig::maxLines();
        entries = 300;
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Range", QString("entries=:-%1:%2").arg(entries - 1).arg(entries).toUtf8());
    } break;
    case RequestType::EntriesUpdate:
        url = m_entriesUrl;
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Range", QString("entries=%1").arg(m_cursor).toUtf8());
    default:
        break;
    }

    request.setUrl(url);
    logDebug() << "Journal network analyzer requested" << url;
    m_reply = m_networkManager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(httpFinished()));
    connect(m_reply, SIGNAL(readyRead()), SLOT(httpReadyRead()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
}
