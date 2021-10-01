/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *   SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
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
#include "logFile.h"
#include "logViewModel.h"
#include "logging.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRegularExpression>

#include <KLocalizedString>

JournaldNetworkAnalyzer::JournaldNetworkAnalyzer(LogMode *mode, const JournaldAnalyzerOptions &options)
    : JournaldAnalyzer(mode)
{
    mAddress = options.address;

    connect(&mNetworkManager, &QNetworkAccessManager::sslErrors, this, &JournaldNetworkAnalyzer::sslErrors);

    auto *configuration = mode->logModeConfiguration<JournaldConfiguration *>();

    mBaseUrl = QStringLiteral("%1://%2:%3/").arg(mAddress.https ? QStringLiteral("https") : QStringLiteral("http")).arg(mAddress.address).arg(mAddress.port);

    mEntriesUrlUpdating = mBaseUrl + QStringLiteral("entries");
    mEntriesUrlFull = mEntriesUrlUpdating;

    QString filterPrefix;
    if (configuration->displayCurrentBootOnly()) {
        mEntriesUrlUpdating.append(QStringLiteral("?boot&follow"));
        mEntriesUrlFull.append(QStringLiteral("?boot"));
        filterPrefix = QStringLiteral("&");
    } else {
        mEntriesUrlUpdating.append(QStringLiteral("?follow"));
        filterPrefix = QStringLiteral("?");
    }

    if (!options.filter.isEmpty()) {
        mEntriesUrlUpdating.append(QStringLiteral("&") + options.filter);
        mEntriesUrlFull.append(filterPrefix + options.filter);
    }

    mSyslogIdUrl = mBaseUrl + QStringLiteral("fields/SYSLOG_IDENTIFIER");
    mSystemdUnitsUrl = mBaseUrl + QStringLiteral("fields/_SYSTEMD_UNIT");

    mFilterName = options.filter.section(QChar::fromLatin1('='), 1);

    mReply = nullptr;
}

void JournaldNetworkAnalyzer::watchLogFiles(bool enabled)
{
    if (enabled) {
        sendRequest(RequestType::SyslogIds);
    } else {
        mCursor.clear();
        updateStatus(QString());
        if (mReply) {
            mReply->abort();
            mReply->deleteLater();
            mReply = nullptr;
        }
    }
}

QStringList JournaldNetworkAnalyzer::units() const
{
    return mSystemdUnits;
}

QStringList JournaldNetworkAnalyzer::syslogIdentifiers() const
{
    return mSyslogIdentifiers;
}

void JournaldNetworkAnalyzer::httpFinished()
{
    QByteArray data = mReply->readAll();
    if (mCurrentRequest == RequestType::EntriesFull) {
        if (data.size()) {
            parseEntries(data, FullRead);
            updateStatus(i18n("Connected"));
        }
        if (!mCursor.isEmpty()) {
            sendRequest(RequestType::EntriesUpdate);
        } else {
            logWarning() << "Network journal analyzer failed to extract cursor string. "
                            "Journal updates will be unavailable.";
        }
    } else {
        QString identifiersString = QString::fromUtf8(data);
        QStringList identifiersList = identifiersString.split(QChar::fromLatin1('\n'), Qt::SkipEmptyParts);
        switch (mCurrentRequest) {
        case RequestType::SyslogIds:
            mSyslogIdentifiers = identifiersList;
            mSyslogIdentifiers.sort();
            sendRequest(RequestType::Units);
            break;
        case RequestType::Units: {
            mSystemdUnits = identifiersList;
            mSystemdUnits.sort();
            auto *journalLogMode = dynamic_cast<JournaldLogMode *>(mLogMode);
            JournalFilters filters;
            filters.syslogIdentifiers = mSyslogIdentifiers;
            filters.systemdUnits = mSystemdUnits;
            journalLogMode->updateJournalFilters(mAddress, filters);
            // Regenerate the "Logs" submenu to include new syslog identifiers and systemd units.
            Q_EMIT mLogMode->menuChanged();
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
    if (mCurrentRequest == RequestType::EntriesUpdate) {
        QByteArray data = mReply->readAll();
        parseEntries(data, UpdatingRead);
    }
}

void JournaldNetworkAnalyzer::httpError(QNetworkReply::NetworkError code)
{
    if (mParsingPaused) {
        return;
    }

    if (code == QNetworkReply::OperationCanceledError) {
        return;
    }

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
    if (mParsingPaused) {
        logDebug() << "Parsing is paused, discarding journald entries.";
        return;
    }

    QList<QByteArray> items = data.split('{');
    QList<JournalEntry> entries;
    for (int i = 0; i < items.size(); i++) {
        QByteArray &item = items[i];
        if (item.isEmpty()) {
            continue;
        }
        item.prepend('{');
        QJsonParseError jsonError{};
        QJsonDocument doc = QJsonDocument::fromJson(item, &jsonError);
        if (jsonError.error != 0) {
            continue;
        }
        QJsonObject object = doc.object();

        if ((readingMode == FullRead) && (i == items.size() - 1)) {
            mCursor = object[QStringLiteral("__CURSOR")].toString();
            break;
        }

        JournalEntry entry;
        auto timestampUsec = object[QStringLiteral("__REALTIME_TIMESTAMP")].toVariant().value<quint64>();
        entry.date.setMSecsSinceEpoch(timestampUsec / 1000);
        entry.message = object[QStringLiteral("MESSAGE")].toString();
        if (entry.message.isEmpty()) {
            // MESSAGE field contains a JSON array of bytes.
            QByteArray stringBytes;
            QJsonArray a = object[QStringLiteral("MESSAGE")].toArray();
            for (int i = 0; i < a.size(); i++) {
                stringBytes.append(a[i].toVariant().value<char>());
            }
            entry.message = QString::fromUtf8(stringBytes);
        }
        entry.message.remove(QRegularExpression(QLatin1String(ConsoleColorEscapeSequence)));
        entry.priority = object[QStringLiteral("PRIORITY")].toVariant().value<int>();
        entry.bootID = object[QStringLiteral("_BOOT_ID")].toString();
        QString unit = object[QStringLiteral("SYSLOG_IDENTIFIER")].toString();
        if (unit.isEmpty()) {
            unit = object[QStringLiteral("_SYSTEMD_UNIT")].toString();
        }
        entry.unit = unit;

        entries << entry;
    }

    if (entries.empty()) {
        logDebug() << "Received no entries.";
    } else {
        mInsertionLocking.lock();
        mLogViewModel->startingMultipleInsertions();

        if (FullRead == readingMode) {
            Q_EMIT statusBarChanged(i18n("Reading journald entries..."));
            // Start displaying the loading bar.
            Q_EMIT readFileStarted(*mLogMode, LogFile(), 0, 1);
        }

        // Add journald entries to the model.
        int entriesInserted = updateModel(entries, readingMode);

        mLogViewModel->endingMultipleInsertions(readingMode, entriesInserted);

        if (FullRead == readingMode) {
            Q_EMIT statusBarChanged(i18n("Journald entries loaded successfully."));

            // Stop displaying the loading bar.
            Q_EMIT readEnded();
        }

        // Inform LogManager that new lines have been added.
        Q_EMIT logUpdated(entriesInserted);

        mInsertionLocking.unlock();
    }
}

void JournaldNetworkAnalyzer::sendRequest(RequestType requestType)
{
    if (mReply) {
        mReply->deleteLater();
    }

    mCurrentRequest = requestType;

    QNetworkRequest request;
    QString url;

    switch (requestType) {
    case RequestType::SyslogIds:
        url = mSyslogIdUrl;
        break;
    case RequestType::Units:
        url = mSystemdUnitsUrl;
        break;
    case RequestType::EntriesFull: {
        url = mEntriesUrlFull;
        int entries = KSystemLogConfig::maxLines();
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Range", QStringLiteral("entries=:-%1:%2").arg(entries - 1).arg(entries).toUtf8());
    } break;
    case RequestType::EntriesUpdate:
        url = mEntriesUrlUpdating;
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Range", QStringLiteral("entries=%1").arg(mCursor).toUtf8());
    default:
        break;
    }

    request.setUrl(QUrl(url));
    logDebug() << "Journal network analyzer requested" << url;
    mReply = mNetworkManager.get(request);
    connect(mReply, &QNetworkReply::finished, this, &JournaldNetworkAnalyzer::httpFinished);
    connect(mReply, &QNetworkReply::readyRead, this, &JournaldNetworkAnalyzer::httpReadyRead);
    connect(mReply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::errorOccurred), this, &JournaldNetworkAnalyzer::httpError);
}

void JournaldNetworkAnalyzer::updateStatus(const QString &status)
{
    QString newStatus = mBaseUrl;
    if (!mFilterName.isEmpty()) {
        newStatus += QLatin1String(" - ") + mFilterName;
    }
    if (!status.isEmpty()) {
        newStatus += QLatin1String(" - ") + status;
    }
    Q_EMIT statusChanged(newStatus);
}
