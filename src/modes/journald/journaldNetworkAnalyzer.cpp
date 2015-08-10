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
    m_url = QString("http://%1:%2/entries?").arg(address).arg(port);
    if (configuration->displayCurrentBootOnly())
        m_url.append("boot&");
    m_url.append("follow");
    if (!filter.isEmpty())
        m_url.append("&" + filter);
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
        QNetworkRequest request(m_url);
        request.setRawHeader("Accept", "application/json");
        m_reply = m_networkManager.get(request);
        connect(m_reply, SIGNAL(finished()), SLOT(httpFinished()));
        connect(m_reply, SIGNAL(readyRead()), SLOT(httpReadyRead()));
        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                SLOT(error(QNetworkReply::NetworkError)));
        connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)),
                SLOT(updateDataReadProgress(qint64, qint64)));
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
    logDebug() << "Finished!";
}

void JournaldNetworkAnalyzer::httpReadyRead()
{
    QByteArray chunk = m_reply->readAll();
    logDebug() << "httpReadyRead" << m_data.size();
    logDebug() << chunk;

//    m_data.append(m_reply->readAll());
//    logDebug() << "httpReadyRead" << m_data.size();
}

void JournaldNetworkAnalyzer::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    logDebug() << "updateDataReadProgress" << bytesRead << totalBytes;
}

void JournaldNetworkAnalyzer::error(QNetworkReply::NetworkError code)
{
    logWarning() << "Network error:" << code;
}
