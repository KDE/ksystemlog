/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
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

#ifndef _CUPS_ITEM_BUILDER_H_
#define _CUPS_ITEM_BUILDER_H_

#include <QString>

#include "logModeItemBuilder.h"

#include <klocale.h>

#include "logging.h"

#include "logLine.h"
#include "logViewWidgetItem.h"
#include "logMode.h"

class CupsItemBuilder : public LogModeItemBuilder {

	public:
		CupsItemBuilder() {
			
		}
		
		virtual ~CupsItemBuilder() {
			
		}

		QString createFormattedText(LogLine* line) const {
			QString result;
			
			result.append("<table>");
			
			result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
			result.append(labelMessageFormat(i18n("Level:"), line->logLevel()->name()));

			result.append("</table>");

			return result;
		}
};


#endif // _CUPS_ITEM_BUILDER_H_
