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

#include "journaldConfiguration.h"

JournaldConfiguration::JournaldConfiguration()
{
    m_displayCurrentBootOnly = true;
    m_entriesType = EntriesAll;

    configuration->setCurrentGroup(QLatin1String("JournaldLogMode"));
    configuration->addItemBool(QLatin1String("CurrentBootOnly"), m_displayCurrentBootOnly, true);
    configuration->addItemInt(QLatin1String("EntriesType"), m_entriesType);
    configuration->addItemStringList(QLatin1String("RemoteJournals"), m_remoteJournals);
}

bool JournaldConfiguration::displayCurrentBootOnly() const
{
    return m_displayCurrentBootOnly;
}

void JournaldConfiguration::setDisplayCurrentBootOnly(bool displayCurrentBootOnly)
{
    m_displayCurrentBootOnly = displayCurrentBootOnly;
}

JournaldConfiguration::EntriesType JournaldConfiguration::entriesType()
{
    if ((m_entriesType < EntriesAll) || (m_entriesType > EntriesSystem))
        m_entriesType = EntriesAll;
    return static_cast<EntriesType>(m_entriesType);
}

void JournaldConfiguration::setEntriesType(JournaldConfiguration::EntriesType entriesType)
{
    m_entriesType = entriesType;
}

QList<JournalAddress> JournaldConfiguration::remoteJournals() const
{
    QList<JournalAddress> journals;
    for (const QString &addressItem : m_remoteJournals) {
        JournalAddress addressInfo;
        addressInfo.address = addressItem.section(QChar::fromLatin1('|'), 0, 0);
        addressInfo.port = addressItem.section(QChar::fromLatin1('|'), 1, 1).toUInt();
        const int https = addressItem.section(QChar::fromLatin1('|'), 2).toInt();
        addressInfo.https = https != 0;
        journals.append(addressInfo);
    }
    return journals;
}

void JournaldConfiguration::setRemoteJournals(const QList<JournalAddress> &remoteJournals)
{
    m_remoteJournals.clear();
    for (const JournalAddress &addressInfo : remoteJournals) {
        m_remoteJournals.append(QStringLiteral("%1|%2|%3")
                                    .arg(addressInfo.address)
                                    .arg(addressInfo.port)
                                    .arg(addressInfo.https ? 1 : 0));
    }
}
