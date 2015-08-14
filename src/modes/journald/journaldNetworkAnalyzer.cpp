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
    : Analyzer(logMode)
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

LogViewColumns JournaldNetworkAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, true));
    columns.addColumn(LogViewColumn(i18n("Unit"), true, true));
    columns.addColumn(LogViewColumn(i18n("Message"), true, true));
    return columns;
}

void JournaldNetworkAnalyzer::setLogFiles(const QList<LogFile> &logFiles)
{
    Q_UNUSED(logFiles)
    // Do nothing.
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

void JournaldNetworkAnalyzer::httpFinished()
{
    QByteArray data = m_reply->readAll();
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
        sendRequest(RequestType::Entries);
        break;
    default:
        break;
    }
}

void JournaldNetworkAnalyzer::httpReadyRead()
{
    if (m_currentRequest == RequestType::Entries) {
        QByteArray chunk = m_reply->readAll();
        logDebug() << chunk;

        //    m_data.append(m_reply->readAll());
        //    logDebug() << "httpReadyRead" << m_data.size();
    }
}

void JournaldNetworkAnalyzer::error(QNetworkReply::NetworkError code)
{
    // TODO: handle errors
    logWarning() << "Network error:" << code;
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
    case RequestType::Entries: {
        url = m_entriesUrl;
        int entries = KSystemLogConfig::maxLines();
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Range", QString("entries=:-%1:%2").arg(entries - 1).arg(entries).toUtf8());
    } break;
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
