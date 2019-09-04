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

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRegularExpression>

#include <KLocalizedString>

JournaldNetworkAnalyzer::JournaldNetworkAnalyzer(LogMode *logMode, const JournaldAnalyzerOptions &options)
    : JournaldAnalyzer(logMode)
{
    m_address = options.address;

    connect(&m_networkManager, &QNetworkAccessManager::sslErrors, this, &JournaldNetworkAnalyzer::sslErrors);

    JournaldConfiguration *configuration = logMode->logModeConfiguration<JournaldConfiguration *>();

    m_baseUrl = QStringLiteral("%1://%2:%3/")
                    .arg(m_address.https ? QStringLiteral("https") : QStringLiteral("http"))
                    .arg(m_address.address)
                    .arg(m_address.port);

    m_entriesUrlUpdating = m_baseUrl + QStringLiteral("entries");
    m_entriesUrlFull = m_entriesUrlUpdating;

    QString filterPrefix;
    if (configuration->displayCurrentBootOnly()) {
        m_entriesUrlUpdating.append(QStringLiteral("?boot&follow"));
        m_entriesUrlFull.append(QStringLiteral("?boot"));
        filterPrefix = QStringLiteral("&");
    } else {
        m_entriesUrlUpdating.append(QStringLiteral("?follow"));
        filterPrefix = QStringLiteral("?");
    }

    if (!options.filter.isEmpty()) {
        m_entriesUrlUpdating.append(QStringLiteral("&") + options.filter);
        m_entriesUrlFull.append(filterPrefix + options.filter);
    }

    m_syslogIdUrl = m_baseUrl + QStringLiteral("fields/SYSLOG_IDENTIFIER");
    m_systemdUnitsUrl = m_baseUrl + QStringLiteral("fields/_SYSTEMD_UNIT");

    m_filterName = options.filter.section(QChar::fromLatin1('='), 1);

    m_reply = nullptr;
}

JournaldNetworkAnalyzer::~JournaldNetworkAnalyzer()
{
}

void JournaldNetworkAnalyzer::watchLogFiles(bool enabled)
{
    if (enabled) {
        sendRequest(RequestType::SyslogIds);
    } else {
        m_cursor.clear();
        updateStatus(QString());
        if (m_reply) {
            m_reply->abort();
            m_reply->deleteLater();
            m_reply = nullptr;
        }
    }
}

QStringList JournaldNetworkAnalyzer::units() const
{
    return m_systemdUnits;
}

QStringList JournaldNetworkAnalyzer::syslogIdentifiers() const
{
    return m_syslogIdentifiers;
}

void JournaldNetworkAnalyzer::httpFinished()
{
    QByteArray data = m_reply->readAll();
    if (m_currentRequest == RequestType::EntriesFull) {
        if (data.size()) {
            parseEntries(data, FullRead);
            updateStatus(i18n("Connected"));
        }
        if (!m_cursor.isEmpty())
            sendRequest(RequestType::EntriesUpdate);
        else {
            logWarning() << "Network journal analyzer failed to extract cursor string. "
                            "Journal updates will be unavailable.";
        }
    } else {
        QString identifiersString = QString::fromUtf8(data);
        QStringList identifiersList = identifiersString.split(QChar::fromLatin1('\n'), QString::SkipEmptyParts);
        switch (m_currentRequest) {
        case RequestType::SyslogIds:
            m_syslogIdentifiers = identifiersList;
            m_syslogIdentifiers.sort();
            sendRequest(RequestType::Units);
            break;
        case RequestType::Units: {
            m_systemdUnits = identifiersList;
            m_systemdUnits.sort();
            JournaldLogMode *journalLogMode = dynamic_cast<JournaldLogMode *>(logMode);
            JournalFilters filters;
            filters.syslogIdentifiers = m_syslogIdentifiers;
            filters.systemdUnits = m_systemdUnits;
            journalLogMode->updateJournalFilters(m_address, filters);
            // Regenerate the "Logs" submenu to include new syslog identifiers and systemd units.
            emit logMode->menuChanged();
            sendRequest(RequestType::EntriesFull);
            break;
        }
        default:
            break;
        }
    }
}

void JournaldNetworkAnalyzer::httpReadyRead()
{
    if (m_currentRequest == RequestType::EntriesUpdate) {
        QByteArray data = m_reply->readAll();
        parseEntries(data, UpdatingRead);
    }
}

void JournaldNetworkAnalyzer::httpError(QNetworkReply::NetworkError code)
{
    if (parsingPaused)
        return;

    if (code == QNetworkReply::OperationCanceledError)
        return;

    updateStatus(i18n("Connection error"));
    logWarning() << "Network journald connection error:" << code;
}

void JournaldNetworkAnalyzer::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    Q_UNUSED(errors)
    reply->ignoreSslErrors();
}

void JournaldNetworkAnalyzer::parseEntries(QByteArray &data, Analyzer::ReadingMode readingMode)
{
    if (parsingPaused) {
        logDebug() << "Parsing is paused, discarding journald entries.";
        return;
    }

    QList<QByteArray> items = data.split('{');
    QList<JournalEntry> entries;
    for (int i = 0; i < items.size(); i++) {
        QByteArray &item = items[i];
        if (item.isEmpty())
            continue;
        item.prepend('{');
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(item, &jsonError);
        if (jsonError.error != 0)
            continue;
        QJsonObject object = doc.object();

        if ((readingMode == FullRead) && (i == items.size() - 1)) {
            m_cursor = object[QStringLiteral("__CURSOR")].toString();
            break;
        }

        JournalEntry entry;
        quint64 timestampUsec = object[QStringLiteral("__REALTIME_TIMESTAMP")].toVariant().value<quint64>();
        entry.date.setMSecsSinceEpoch(timestampUsec / 1000);
        entry.message = object[QStringLiteral("MESSAGE")].toString();
        if (entry.message.isEmpty()) {
            // MESSAGE field contains a JSON array of bytes.
            QByteArray stringBytes;
            QJsonArray a = object[QStringLiteral("MESSAGE")].toArray();
            for (int i = 0; i < a.size(); i++)
                stringBytes.append(a[i].toVariant().value<char>());
            entry.message = QString::fromUtf8(stringBytes);
        }
        entry.message.remove(QRegularExpression(QLatin1String(ConsoleColorEscapeSequence)));
        entry.priority = object[QStringLiteral("PRIORITY")].toVariant().value<int>();
        entry.bootID = object[QStringLiteral("_BOOT_ID")].toString();
        QString unit = object[QStringLiteral("SYSLOG_IDENTIFIER")].toString();
        if (unit.isEmpty())
            unit = object[QStringLiteral("_SYSTEMD_UNIT")].toString();
        entry.unit = unit;

        entries << entry;
    }

    if (entries.size() == 0) {
        logDebug() << "Received no entries.";
    } else {
        insertionLocking.lock();
        logViewModel->startingMultipleInsertions();

        if (FullRead == readingMode) {
            emit statusBarChanged(i18n("Reading journald entries..."));
            // Start displaying the loading bar.
            emit readFileStarted(*logMode, LogFile(), 0, 1);
        }

        // Add journald entries to the model.
        int entriesInserted = updateModel(entries, readingMode);

        logViewModel->endingMultipleInsertions(readingMode, entriesInserted);

        if (FullRead == readingMode) {
            emit statusBarChanged(i18n("Journald entries loaded successfully."));

            // Stop displaying the loading bar.
            emit readEnded();
        }

        // Inform LogManager that new lines have been added.
        emit logUpdated(entriesInserted);

        insertionLocking.unlock();
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
        url = m_entriesUrlFull;
        int entries = KSystemLogConfig::maxLines();
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Range", QStringLiteral("entries=:-%1:%2").arg(entries - 1).arg(entries).toUtf8());
    } break;
    case RequestType::EntriesUpdate:
        url = m_entriesUrlUpdating;
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Range", QStringLiteral("entries=%1").arg(m_cursor).toUtf8());
    default:
        break;
    }

    request.setUrl(QUrl(url));
    logDebug() << "Journal network analyzer requested" << url;
    m_reply = m_networkManager.get(request);
    connect(m_reply, &QNetworkReply::finished, this, &JournaldNetworkAnalyzer::httpFinished);
    connect(m_reply, &QNetworkReply::readyRead, this, &JournaldNetworkAnalyzer::httpReadyRead);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &JournaldNetworkAnalyzer::httpError);
}

void JournaldNetworkAnalyzer::updateStatus(QString status)
{
    QString newStatus = m_baseUrl;
    if (!m_filterName.isEmpty()) {
        newStatus += QLatin1String(" - ") + m_filterName;
    }
    if (!status.isEmpty()) {
        newStatus += QLatin1String(" - ") + status;
    }
    emit statusChanged(newStatus);
}
