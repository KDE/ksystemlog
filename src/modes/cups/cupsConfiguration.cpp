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

#include "cupsConfiguration.h"

CupsConfiguration::CupsConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("CupsLogMode"));

    const QStringList defaultCupsPaths{QStringLiteral("/var/log/cups/error_log")};
    mConfiguration->addItemStringList(QStringLiteral("CupsLogFilesPaths"), mCupsPaths, defaultCupsPaths, QStringLiteral("CupsLogFilesPaths"));

    const QStringList defaultCupsAccessPaths{QStringLiteral("/var/log/cups/access_log")};
    mConfiguration->addItemStringList(QStringLiteral("CupsAccessLogFilesPaths"),
                                      mCupsAccessPaths,
                                      defaultCupsAccessPaths,
                                      QStringLiteral("CupsAccessLogFilesPaths"));

    const QStringList defaultCupsPagePaths{QStringLiteral("/var/log/cups/page_log")};
    mConfiguration->addItemStringList(QStringLiteral("CupsPageLogFilesPaths"), mCupsPagePaths, defaultCupsPagePaths, QStringLiteral("CupsPageLogFilesPaths"));

    const QStringList defaultCupsPdfPaths{QStringLiteral("/var/log/cups/cups-pdf_log")};
    mConfiguration->addItemStringList(QStringLiteral("CupsPdfLogFilesPaths"), mCupsPdfPaths, defaultCupsPdfPaths, QStringLiteral("CupsPdfLogFilesPaths"));
}

CupsConfiguration::~CupsConfiguration()
{
}

QStringList CupsConfiguration::cupsPaths() const
{
    return mCupsPaths;
}

QStringList CupsConfiguration::cupsAccessPaths() const
{
    return mCupsAccessPaths;
}

QStringList CupsConfiguration::cupsPagePaths() const
{
    return mCupsPagePaths;
}

QStringList CupsConfiguration::cupsPdfPaths() const
{
    return mCupsPdfPaths;
}

void CupsConfiguration::setCupsPaths(const QStringList &cupsPaths)
{
    mCupsPaths = cupsPaths;
}

void CupsConfiguration::setCupsAccessPaths(const QStringList &cupsAccessPaths)
{
    mCupsAccessPaths = cupsAccessPaths;
}

void CupsConfiguration::setCupsPagePaths(const QStringList &cupsPagePaths)
{
    mCupsPagePaths = cupsPagePaths;
}

void CupsConfiguration::setCupsPdfPaths(const QStringList &cupsPdfPaths)
{
    mCupsPdfPaths = cupsPdfPaths;
}
