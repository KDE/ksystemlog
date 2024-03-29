/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>
    SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "journaldConfiguration.h"

JournaldConfiguration::JournaldConfiguration()
{
    mDisplayCurrentBootOnly = true;
    mEntriesType = EntriesAll;

    mConfiguration->setCurrentGroup(QLatin1String("JournaldLogMode"));
    mConfiguration->addItemBool(QLatin1String("CurrentBootOnly"), mDisplayCurrentBootOnly, true);
    mConfiguration->addItemInt(QLatin1String("EntriesType"), mEntriesType);
    mConfiguration->addItemStringList(QLatin1String("RemoteJournals"), mRemoteJournals);
}

bool JournaldConfiguration::displayCurrentBootOnly() const
{
    return mDisplayCurrentBootOnly;
}

void JournaldConfiguration::setDisplayCurrentBootOnly(bool displayCurrentBootOnly)
{
    mDisplayCurrentBootOnly = displayCurrentBootOnly;
}

JournaldConfiguration::EntriesType JournaldConfiguration::entriesType()
{
    if ((mEntriesType < EntriesAll) || (mEntriesType > EntriesSystem)) {
        mEntriesType = EntriesAll;
    }
    return static_cast<EntriesType>(mEntriesType);
}

void JournaldConfiguration::setEntriesType(JournaldConfiguration::EntriesType entriesType)
{
    mEntriesType = entriesType;
}

QVector<JournalAddress> JournaldConfiguration::remoteJournals() const
{
    QVector<JournalAddress> journals;
    for (const QString &addressItem : std::as_const(mRemoteJournals)) {
        JournalAddress addressInfo;
        addressInfo.address = addressItem.section(QChar::fromLatin1('|'), 0, 0);
        addressInfo.port = addressItem.section(QChar::fromLatin1('|'), 1, 1).toUInt();
        const int https = addressItem.section(QChar::fromLatin1('|'), 2).toInt();
        addressInfo.https = https != 0;
        journals.append(addressInfo);
    }
    return journals;
}

void JournaldConfiguration::setRemoteJournals(const QVector<JournalAddress> &remoteJournals)
{
    mRemoteJournals.clear();
    for (const JournalAddress &addressInfo : remoteJournals) {
        mRemoteJournals.append(QStringLiteral("%1|%2|%3").arg(addressInfo.address).arg(addressInfo.port).arg(addressInfo.https ? 1 : 0));
    }
}

#include "moc_journaldConfiguration.cpp"
