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
