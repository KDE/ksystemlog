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

#include "parsingHelper.h"

#include <klocale.h>

#include "logging.h"


ParsingHelper* ParsingHelper::self = NULL;

ParsingHelper* ParsingHelper::instance() {
	if (ParsingHelper::self == NULL) {
		ParsingHelper::self = new ParsingHelper();
	}

	return ParsingHelper::self;
}

ParsingHelper::ParsingHelper() {
	//Initialize Existing months
	mapMonths[QLatin1String( "Jan" )]=1;
	mapMonths[QLatin1String( "Feb" )]=2;
	mapMonths[QLatin1String( "Mar" )]=3;
	mapMonths[QLatin1String( "Apr" )]=4;
	mapMonths[QLatin1String( "May" )]=5;
	mapMonths[QLatin1String( "Jun" )]=6;
	mapMonths[QLatin1String( "Jul" )]=7;
	mapMonths[QLatin1String( "Aug" )]=8;
	mapMonths[QLatin1String( "Sep" )]=9;
	mapMonths[QLatin1String( "Oct" )]=10;
	mapMonths[QLatin1String( "Nov" )]=11;
	mapMonths[QLatin1String( "Dec" )]=12;

	//Initialize HTTP Responses
	//1xx Responses
	mapHTTPResponse[QLatin1String( "100" )]=QLatin1String( "Continue" );
	mapHTTPResponse[QLatin1String( "101" )]=QLatin1String( "Switching Protocols" );

	//2xx Responses
	mapHTTPResponse[QLatin1String( "200" )]=QLatin1String( "OK" );
	mapHTTPResponse[QLatin1String( "201" )]=QLatin1String( "Created" );
	mapHTTPResponse[QLatin1String( "202" )]=QLatin1String( "Accepted" );
	mapHTTPResponse[QLatin1String( "203" )]=QLatin1String( "Non-Authoritative Information" );
	mapHTTPResponse[QLatin1String( "204" )]=QLatin1String( "No Content" );
	mapHTTPResponse[QLatin1String( "205" )]=QLatin1String( "Reset Content" );
	mapHTTPResponse[QLatin1String( "206" )]=QLatin1String( "Partial Content" );

	//3xx Responses
	mapHTTPResponse[QLatin1String( "300" )]=QLatin1String( "OK" );
	mapHTTPResponse[QLatin1String( "301" )]=QLatin1String( "Moved Permanently" );
	mapHTTPResponse[QLatin1String( "302" )]=QLatin1String( "Found" );
	mapHTTPResponse[QLatin1String( "303" )]=QLatin1String( "See Other" );
	mapHTTPResponse[QLatin1String( "304" )]=QLatin1String( "Not Modified" );
	mapHTTPResponse[QLatin1String( "305" )]=QLatin1String( "Use Proxy" );
	mapHTTPResponse[QLatin1String( "306" )]=QLatin1String( "(Unused)" );
	mapHTTPResponse[QLatin1String( "307" )]=QLatin1String( "Temporary Redirect" );

	//4xx Responses
	mapHTTPResponse[QLatin1String( "400" )]=QLatin1String( "Bad Request" );
	mapHTTPResponse[QLatin1String( "401" )]=QLatin1String( "Unauthorized" );
	mapHTTPResponse[QLatin1String( "402" )]=QLatin1String( "Payment Required" );
	mapHTTPResponse[QLatin1String( "403" )]=QLatin1String( "Forbidden" );
	mapHTTPResponse[QLatin1String( "404" )]=QLatin1String( "Not Found" );
	mapHTTPResponse[QLatin1String( "405" )]=QLatin1String( "Method Not Allowed" );
	mapHTTPResponse[QLatin1String( "406" )]=QLatin1String( "Not Acceptable" );
	mapHTTPResponse[QLatin1String( "407" )]=QLatin1String( "Proxy Authentication Required" );
	mapHTTPResponse[QLatin1String( "408" )]=QLatin1String( "Request Timeout" );
	mapHTTPResponse[QLatin1String( "409" )]=QLatin1String( "Conflict" );
	mapHTTPResponse[QLatin1String( "410" )]=QLatin1String( "Gone" );
	mapHTTPResponse[QLatin1String( "411" )]=QLatin1String( "Length Required" );
	mapHTTPResponse[QLatin1String( "412" )]=QLatin1String( "Precondition Failed" );
	mapHTTPResponse[QLatin1String( "413" )]=QLatin1String( "Request Entity Too Large" );
	mapHTTPResponse[QLatin1String( "414" )]=QLatin1String( "Request-URI Too Long" );
	mapHTTPResponse[QLatin1String( "415" )]=QLatin1String( "Unsupported Media Type" );
	mapHTTPResponse[QLatin1String( "416" )]=QLatin1String( "Requested Range Not Satisfiable" );
	mapHTTPResponse[QLatin1String( "417" )]=QLatin1String( "Expectation Failed" );

	//5xx Responses
	mapHTTPResponse[QLatin1String( "500" )]=QLatin1String( "Internal Server Error" );
	mapHTTPResponse[QLatin1String( "501" )]=QLatin1String( "Not Implemented" );
	mapHTTPResponse[QLatin1String( "502" )]=QLatin1String( "Bad Gateway" );
	mapHTTPResponse[QLatin1String( "503" )]=QLatin1String( "Service Unavailable" );
	mapHTTPResponse[QLatin1String( "504" )]=QLatin1String( "Gateway Timeout" );
	mapHTTPResponse[QLatin1String( "505" )]=QLatin1String( "HTTP Version Not Supported" );

}

ParsingHelper::~ParsingHelper() {

}

QDateTime ParsingHelper::parseHttpDateTime(const QString& logLine) {
	//Format example : 22/May/2005:01:50:34 +0200

	QString day=logLine.mid(0,2);
	QString month=logLine.mid(3,3);
	QString year=logLine.mid(7,4);

	QString hour=logLine.mid(12,2);
	QString min=logLine.mid(15,2);
	QString sec=logLine.mid(18,2);

	//QString zone=logLine.mid(22,5);

	return QDateTime(QDate(year.toInt(), parseSyslogMonth(month), day.toInt()), QTime(hour.toInt(), min.toInt(), sec.toInt()));
}

/**
 * Example : "Oct  1 09:11:45 2005"
 */
QDateTime ParsingHelper::parseSyslogDateTime(const QString& dateTime) {
	//TODO Create this regexp in constructor
	QRegExp regex(QLatin1String( "(\\S*)[ ]+(\\d*) (\\d*):(\\d*):(\\d*) (\\d*)" ));

	int firstPosition = regex.indexIn(dateTime);
	if (firstPosition == -1) {
		logDebug() << "Unable to parse date " << dateTime << endl;
		return QDateTime::currentDateTime();
	}

	return QDateTime(QDate(regex.cap(6).toInt(), parseSyslogMonth(regex.cap(1)), regex.cap(2).toInt()), QTime(regex.cap(3).toInt(), regex.cap(4).toInt(), regex.cap(5).toInt(), 0));
}

QString ParsingHelper::syslogDateTimeRegexp() {
	return QLatin1String( "(\\S*[ ]+\\d* \\d*:\\d*:\\d* \\d*)" );
}

int ParsingHelper::parseSyslogMonth(const QString& string) {
	return mapMonths.value(string, 1);

}

QString ParsingHelper::parseSize(const QString& stringSize) {
	qint64 size=stringSize.toLongLong();

	return KGlobal::locale()->formatByteSize(size);
}

QString ParsingHelper::parseHttpResponse(const QString& response) {
	//Search the response string in the map
	QMap<QString, QString>::Iterator it=mapHTTPResponse.find(response);
	if (it!=mapHTTPResponse.end())
		return i18nc("HttpResponseNumber HttpResponseDescription", "%1 %2", response, *it);
	else
		return response;

}

QString ParsingHelper::parseAgent(const QString& agent) {
	return agent;
}
