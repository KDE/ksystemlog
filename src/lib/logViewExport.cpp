/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logViewExport.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QUrlQuery>

#include <KLocalizedString>
#include <KMessageBox>
#include <QPrintPreviewDialog>

#include "ksystemlog_debug.h"

#include "logLine.h"
#include "logViewWidget.h"
#include "logViewWidgetItem.h"

#include "levelPrintPage.h"

LogViewExport::LogViewExport(QWidget *parent, LogViewWidget *logViewWidget)
    : mParent(parent)
    , mLogViewWidget(logViewWidget)
{
}

LogViewExport::~LogViewExport()
{
}

void LogViewExport::sendMail()
{
    qCDebug(KSYSTEMLOG) << "Exporting to mail...";

    QString body(i18n("Here are my logs:\n"));

    body += i18n("---------------------------------------\n");

    int i = 0;
    QTreeWidgetItemIterator it(mLogViewWidget, QTreeWidgetItemIterator::Selected);
    while (*it) {
        auto item = static_cast<LogViewWidgetItem *>(*it);

        body += item->logLine()->exportToText();
        body += QLatin1Char('\n');

        ++it;
        ++i;
    }

    body += i18n("---------------------------------------\n");

    // Too much lines selected
    if (i > 1000) {
        KMessageBox::error(mParent, i18n("You have selected too many lines. Please only select important log lines."), i18n("Too Many Lines Selected"));
        return;
    }

    const QUrlQuery urlQuery = {{QStringLiteral("subject"), i18n("Log Lines of my problem")}, {QStringLiteral("body"), body}};
    QUrl url;
    url.setScheme(QStringLiteral("mailto"));
    url.setQuery(urlQuery);

    QDesktopServices::openUrl(url);
}

void LogViewExport::print(QPrinter *printer)
{
    // create a painter to paint on the printer object
    QPainter painter;

    // start painting
    painter.begin(printer);

    QPen const originalPen(painter.pen());

    QPaintDevice *painterDevice = painter.device();
    int const dpiy = painterDevice->logicalDpiY();
    const int margin = (int)((2 / 2.54) * dpiy); // 2 cm margins
    QRect printView(margin, margin, painterDevice->width() - 2 * margin, painterDevice->height() - 2 * margin);

    int page = 1;

    int movement = 0;

    QTreeWidgetItemIterator it(mLogViewWidget, QTreeWidgetItemIterator::Selected);
    while (*it) {
        auto item = static_cast<LogViewWidgetItem *>(*it);
        const QString body = item->logLine()->exportToText();
        painter.setPen(originalPen);
        painter.drawText(printView, Qt::AlignLeft | Qt::TextWordWrap, body);
        const int fontHeight = painter.fontMetrics().height();
        const int lines = painter.fontMetrics().boundingRect(body).width() / printView.width() + 1;
        const int moveBy = (fontHeight + 2) * lines;
        painter.translate(0, moveBy);
        movement += moveBy;
        if (movement + margin >= printView.height()) {
            painter.setPen(originalPen);
            printPageNumber(painter, printView, movement, page, margin);
            printer->newPage();
            page++;
            movement = 0;
        }
        ++it;
    }

    // stop painting, this will automatically send the print data to the printer
    painter.end();
}

void LogViewExport::printSelection()
{
    qCDebug(KSYSTEMLOG) << "Printing selection...";

    QPrinter printer;

    // do some printer initialization
    printer.setFullPage(true);

    /*
     LevelPrintPage* dialogPage = new LevelPrintPage(parent);
     printer.addDialogPage(dialogPage);
     */

    // initialize the printer using the print dialog
    auto printDialog = new QPrintDialog(&printer, mParent);
    if (!printDialog->exec()) {
        delete printDialog;
        return;
    }
    delete printDialog;
    print(&printer);
}

void LogViewExport::printPreview()
{
    qCDebug(KSYSTEMLOG) << "Printing selection...";

    auto dialog = new QPrintPreviewDialog(mParent);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->resize(800, 750);

    connect(dialog, &QPrintPreviewDialog::paintRequested, this, [=](QPrinter *printing) {
        print(printing);
    });

    dialog->open();
}

void LogViewExport::printPageNumber(QPainter &painter, QRect &printView, int movement, int page, int margin)
{
    qCDebug(KSYSTEMLOG) << "Printing page number...";

    painter.translate(0, -movement);
    printView.moveTo(QPoint(margin, printView.height() * page + margin));
    painter.translate(0, -printView.height());
    painter.drawText(printView.right() - painter.fontMetrics().boundingRect(QString::number(page)).width(),
                     printView.bottom() + painter.fontMetrics().ascent() + 5,
                     QString::number(page));
}

void LogViewExport::copyToClipboard()
{
    qCDebug(KSYSTEMLOG) << "Copying to clipboard...";

    int nbCopied = 0;
    QString text;

    QTreeWidgetItemIterator it(mLogViewWidget, QTreeWidgetItemIterator::Selected);
    while (*it) {
        auto item = static_cast<LogViewWidgetItem *>(*it);

        // Copy the item content to the text string
        text.append(item->logLine()->exportToText());
        text.append(QLatin1Char('\n'));

        ++it;
        nbCopied++;
    }

    // Copy text value only if it is not empty
    if (nbCopied == 0) {
        Q_EMIT statusBarChanged(i18n("No items selected. Nothing copied to clipboard."));
    } else {
        // Copy both to clipboard and X11-selection
        QApplication::clipboard()->setText(text, QClipboard::Clipboard);
        QApplication::clipboard()->setText(text, QClipboard::Selection);

        Q_EMIT statusBarChanged(i18np("1 log line copied to clipboard.", "%1 log lines copied to clipboard.", nbCopied));
    }

    qCDebug(KSYSTEMLOG) << "Copied " << nbCopied << " log lines";
}

void LogViewExport::fileSave()
{
    qCDebug(KSYSTEMLOG) << "Saving to a file...";

    QTreeWidgetItemIterator it(mLogViewWidget, QTreeWidgetItemIterator::Selected);

    // No item selected
    if (!(*it)) {
        Q_EMIT statusBarChanged(i18n("No items selected. Please select items to be able to save them."));
        return;
    }

    const QString filename = QFileDialog::getSaveFileName(mParent, i18n("Save selected log entries to..."), QString());
    if (filename.isEmpty()) {
        return;
    }

    auto ioDev = new QFile(filename);

    if (ioDev->open(QIODevice::WriteOnly)) {
        QTextStream stream(ioDev);

        int nbCopied = 0;

        while (*it) {
            auto item = static_cast<LogViewWidgetItem *>(*it);

            // Copy the item content to the stream
            stream << item->logLine()->exportToText() << '\n';

            // Retrieve the next item
            ++it;
            nbCopied++;
        }

        ioDev->close();

        delete ioDev;

        Q_EMIT statusBarChanged(i18np("1 log line saved to '%2'.", "%1 log lines saved to '%2'.", nbCopied, filename));
    } else {
        const QString message(i18n("Unable to save file '%1': Permission Denied.", filename));
        KMessageBox::error(mParent, message, i18n("Unable to save file."));
    }
}

#include "moc_logViewExport.cpp"
