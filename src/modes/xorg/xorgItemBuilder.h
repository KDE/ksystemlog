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

#ifndef _XORG_ITEM_BUILDER_H_
#define _XORG_ITEM_BUILDER_H_

#include <QString>

#include <klocale.h>

#include "logging.h"

#include "logLine.h"
#include "logViewWidgetItem.h"
#include "logMode.h"

#include "logModeItemBuilder.h"

class LogLine;

class XorgItemBuilder : public LogModeItemBuilder {

	public:
		XorgItemBuilder() {
			
		}
		
		virtual ~XorgItemBuilder() {
			
		}
		

		void prepareItem(LogViewWidgetItem* item) const {
			LogLine* line=item->logLine();
				
			item->setText(0, "");
			
			int i=1;
			foreach(const QString &label, line->logItems()) {
				item->setText(i, label);
				i++;
			}
			
			item->setIcon(0, line->logLevel()->pixmap());
		}

		QString createToolTipText(LogLine* line) const {
			QString result;
			
			QListIterator<QString> it(line->logItems());
			
			result.append("<table>");
			
			QString type=it.next();
			if (type.isEmpty())
				result.append(labelMessageFormat(i18n("Type:"), i18n("none")));
			else
				result.append(labelMessageFormat(i18n("Type:"), type ));
			
			result.append(labelMessageFormat(i18n("Original file:"), line->sourceFileName()));

			result.append("</table>");

			return result;
		}


		QString createFormattedText(LogLine* line) const {
			//It uses the same formating than the tool tip
			return createToolTipText(line);
		}

};

#endif // _XORG_ITEM_BUILDER_H_
