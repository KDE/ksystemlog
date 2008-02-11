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
	mapMonths["Jan"]=1;
	mapMonths["Feb"]=2;
	mapMonths["Mar"]=3;
	mapMonths["Apr"]=4;
	mapMonths["May"]=5;
	mapMonths["Jun"]=6;
	mapMonths["Jul"]=7;
	mapMonths["Aug"]=8;
	mapMonths["Sep"]=9;
	mapMonths["Oct"]=10;
	mapMonths["Nov"]=11;
	mapMonths["Dec"]=12;
	
	//Initialize HTTP Responses
	//1xx Responses
	mapHTTPResponse["100"]="Continue";
	mapHTTPResponse["101"]="Switching Protocols";

	//2xx Responses
	mapHTTPResponse["200"]="OK";
	mapHTTPResponse["201"]="Created";
	mapHTTPResponse["202"]="Accepted";
	mapHTTPResponse["203"]="Non-Authoritative Information";
	mapHTTPResponse["204"]="No Content";
	mapHTTPResponse["205"]="Reset Content";
	mapHTTPResponse["206"]="Partial Content";

	//3xx Responses
	mapHTTPResponse["300"]="OK";
	mapHTTPResponse["301"]="Moved Permanently";
	mapHTTPResponse["302"]="Found";
	mapHTTPResponse["303"]="See Other";
	mapHTTPResponse["304"]="Not Modified";
	mapHTTPResponse["305"]="Use Proxy";
	mapHTTPResponse["306"]="(Unused)";
	mapHTTPResponse["307"]="Temporary Redirect";
	
	//4xx Responses
	mapHTTPResponse["400"]="Bad Request";
	mapHTTPResponse["401"]="Unauthorized";
	mapHTTPResponse["402"]="Payment Required";
	mapHTTPResponse["403"]="Forbidden";
	mapHTTPResponse["404"]="Not Found";
	mapHTTPResponse["405"]="Method Not Allowed";
	mapHTTPResponse["406"]="Not Acceptable";
	mapHTTPResponse["407"]="Proxy Authentication Required";
	mapHTTPResponse["408"]="Request Timeout";
	mapHTTPResponse["409"]="Conflict";
	mapHTTPResponse["410"]="Gone";
	mapHTTPResponse["411"]="Length Required";
	mapHTTPResponse["412"]="Precondition Failed";
	mapHTTPResponse["413"]="Request Entity Too Large";
	mapHTTPResponse["414"]="Request-URI Too Long";
	mapHTTPResponse["415"]="Unsupported Media Type";
	mapHTTPResponse["416"]="Requested Range Not Satisfiable";
	mapHTTPResponse["417"]="Expectation Failed";
	
	//5xx Responses
	mapHTTPResponse["500"]="Internal Server Error";
	mapHTTPResponse["501"]="Not Implemented";
	mapHTTPResponse["502"]="Bad Gateway";
	mapHTTPResponse["503"]="Service Unavailable";
	mapHTTPResponse["504"]="Gateway Timeout";
	mapHTTPResponse["505"]="HTTP Version Not Supported";
	
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
	QRegExp regex("(\\S*)[ ]+(\\d*) (\\d*):(\\d*):(\\d*) (\\d*)");
	
	int firstPosition = regex.indexIn(dateTime);
	if (firstPosition == -1) {
		logDebug() << "Unable to parse date " << dateTime << endl;
		return QDateTime::currentDateTime();
	}
	
	return QDateTime(QDate(regex.cap(6).toInt(), parseSyslogMonth(regex.cap(1)), regex.cap(2).toInt()), QTime(regex.cap(3).toInt(), regex.cap(4).toInt(), regex.cap(5).toInt(), 0));
}

QString ParsingHelper::syslogDateTimeRegexp() {
	return "(\\S*[ ]+\\d* \\d*:\\d*:\\d* \\d*)";
}

int ParsingHelper::parseSyslogMonth(const QString& string) {
	return mapMonths.value(string, 1);
	
}

QString ParsingHelper::parseSize(const QString& stringSize) {
	long size=stringSize.toLong();
	
	if (size<1024)
		return i18nc("Size format", "%1 B", size);
	else if (size<1024*1024) {
		double newSize=size / 1024.;
		QString strNewSize;
		strNewSize.sprintf("%0.2f", newSize);
		return i18nc("Size format", "%1 KB", strNewSize);
	}
	else {
		double newSize=size / (1024.*1024.);
		QString strNewSize;
		strNewSize.sprintf("%0.2f", newSize);
		return i18nc("Size format", "%1 MB", strNewSize);
	}
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
