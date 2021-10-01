/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
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

#include "xsessionConfiguration.h"

XSessionConfiguration::XSessionConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("XSessionLogMode"));

    mConfiguration->addItemString(QStringLiteral("LogFilePath"), mXsessionPath, QStringLiteral("~/.xsession-errors"), QStringLiteral("LogFilePath"));

    mConfiguration->addItemBool(QStringLiteral("IgnoreXorgErrors"), mIgnoreXorgErrors, false, QStringLiteral("IgnoreXorgErrors"));

    QStringList defaultXorgErrorKeywords;
    defaultXorgErrorKeywords.append(QStringLiteral("X Error"));
    defaultXorgErrorKeywords.append(QStringLiteral("  Major opcode"));
    defaultXorgErrorKeywords.append(QStringLiteral("  Minor opcode"));
    defaultXorgErrorKeywords.append(QStringLiteral("  Resource id"));
    mConfiguration->addItemStringList(QStringLiteral("XorgErrorKeywords"), mXorgErrorKeywords, defaultXorgErrorKeywords, QStringLiteral("XorgErrorKeywords"));

    const QStringList defaultWarningKeywords{QStringLiteral("WARNING")};
    mConfiguration->addItemStringList(QStringLiteral("WarningKeywords"), mWarningKeywords, defaultWarningKeywords, QStringLiteral("WarningKeywords"));

    const QStringList defaultErrorKeywords{QStringLiteral("ERROR")};
    mConfiguration->addItemStringList(QStringLiteral("ErrorKeywords"), mErrorKeywords, defaultErrorKeywords, QStringLiteral("ErrorKeywords"));
}

XSessionConfiguration::~XSessionConfiguration()
{
}

QStringList XSessionConfiguration::xorgErrorKeywords() const
{
    return mXorgErrorKeywords;
}

bool XSessionConfiguration::isIgnoreXorgErrors() const
{
    return mIgnoreXorgErrors;
}

void XSessionConfiguration::setIgnoreXorgErrors(bool ignore)
{
    mIgnoreXorgErrors = ignore;
}

QString XSessionConfiguration::xsessionPath() const
{
    return mXsessionPath;
}

void XSessionConfiguration::setXSessionPath(const QString &xsessionPath)
{
    mXsessionPath = xsessionPath;
}

QStringList XSessionConfiguration::warningKeywords() const
{
    return mWarningKeywords;
}

QStringList XSessionConfiguration::errorKeywords() const
{
    return mErrorKeywords;
}
