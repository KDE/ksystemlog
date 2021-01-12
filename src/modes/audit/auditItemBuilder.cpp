#include "auditItemBuilder.h"

#include "logLine.h"

#include <KLocalizedString>

QString AuditItemBuilder::createFormattedText(LogLine *line) const
{
    QString result;

    QListIterator<QString> it(line->logItems());

    result.append(QLatin1String("<table>"));
    result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
    result.append(labelMessageFormat(i18n("Level:"), line->logLevel()->name()));
    result.append(QLatin1String("</table>"));

    return result;
}

QString AuditItemBuilder::createToolTipText(LogLine *line) const
{
    return createFormattedText(line);
}
