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

#include <QList>
#include <QMap>
#include <QFile>
#include <QWidget>
#include <QStringList>
#include <QTextStream>

#include <qtest_kde.h>
#include <kio/netaccess.h>

#include "logging.h"

class FindIncompatibleKioTest : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase();

    void find();

private:
    void displayFoundMethods(const QStringList &methods, const QUrl &url, QTextStream &out);

    QStringList headerContent(const QUrl &url);
    QString convertMethod(const QString &method);

    QMap<QString, bool> findMethods(const QStringList &methods, const QUrl &url);
};

void FindIncompatibleKioTest::initTestCase()
{
}

void FindIncompatibleKioTest::find()
{
    QString kdelibs = QLatin1String("/home/nicolas/workspace/kdelibs");
    QString kdebase = QLatin1String("/home/nicolas/workspace/kdebase");

    QString outputPath = QLatin1String("/home/nicolas/compatibleKioSlaves.html");

    QStringList existingMethods;
    existingMethods << QLatin1String("void SlaveBase::openConnection(void)");
    existingMethods << QLatin1String("void SlaveBase::closeConnection(void)");
    existingMethods << QLatin1String("void SlaveBase::stat(QUrl const &)");
    existingMethods << QLatin1String("void SlaveBase::put(QUrl const &, int, bool, bool)");
    existingMethods << QLatin1String("void SlaveBase::special(const QByteArray &)");
    existingMethods << QLatin1String("void SlaveBase::listDir(QUrl const &)");
    existingMethods << QLatin1String("void SlaveBase::get(QUrl const & )");
    existingMethods << QLatin1String("void SlaveBase::open(QUrl const &, QIODevice::OpenMode)");
    existingMethods << QLatin1String("void SlaveBase::read(KIO::filesize_t)");
    existingMethods << QLatin1String("void SlaveBase::write(const QByteArray &)");
    existingMethods << QLatin1String("void SlaveBase::seek(KIO::filesize_t)");
    existingMethods << QLatin1String("void SlaveBase::close()");
    existingMethods << QLatin1String("void SlaveBase::mimetype(QUrl const &url)");
    existingMethods << QLatin1String("void SlaveBase::rename(QUrl const &, QUrl const &, bool)");
    existingMethods << QLatin1String("void SlaveBase::symlink(QString const &, QUrl const &, bool)");
    existingMethods << QLatin1String("void SlaveBase::copy(QUrl const &, QUrl const &, int, bool)");
    existingMethods << QLatin1String("void SlaveBase::del(QUrl const &, bool)");
    existingMethods << QLatin1String("void SlaveBase::setLinkDest(const QUrl &, const QString&)");
    existingMethods << QLatin1String("void SlaveBase::mkdir(QUrl const &, int)");
    existingMethods << QLatin1String("void SlaveBase::chmod(QUrl const &, int)");
    existingMethods << QLatin1String("void SlaveBase::setModificationTime(QUrl const &, const QDateTime&)");
    existingMethods << QLatin1String("void SlaveBase::chown(QUrl const &, const QString &, const QString &)");
    existingMethods << QLatin1String("void SlaveBase::setSubUrl(QUrl const &)");
    existingMethods << QLatin1String("void SlaveBase::multiGet(const QByteArray &)");

    // logDebug() << existingMethods << endl;

    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logError() << "Unable to open the output file" << outputPath << endl;
        return;
    }

    QTextStream out(&file);

    out << "<html>" << endl;
    out << "<head>" << endl;
    out << "<style type='text/css'>" << endl;
    out << ".found { color:green }" << endl;
    out << ".notFound { color:red }" << endl;
    out << "</style>" << endl;
    out << "</head>" << endl;
    out << "<body>" << endl;

    out << "<div style='text-align:center; font-size:26px; font-variant:small-caps; margin-bottom:40px'>KIO "
           "Slaves Analyzing</div>" << endl;
    out << "<div style='font-style:italic'>Generated by <a "
           "href='http://websvn.kde.org/trunk/playground/sysadmin/ksystemlog/tests/"
           "findIncompatibleKioTest.cpp'>http://websvn.kde.org/trunk/playground/sysadmin/ksystemlog/tests/"
           "findIncompatibleKioTest.cpp</a></div>" << endl;

    QList<QUrl> parsingUrls;

    parsingUrls.append(QUrl::fromLocalFile(kdelibs + QLatin1String("/kioslave/http/http.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdelibs + QLatin1String("/kioslave/ftp/ftp.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdelibs + QLatin1String("/kioslave/file/file.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdelibs + QLatin1String("/kioslave/metainfo/metainfo.h")));

    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/about/kio_about.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/cgi/cgi.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/filter/filter.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/finger/kio_finger.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/fish/fish.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/floppy/kio_floppy.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/info/info.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/man/kio_man.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/media/kio_media.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/nfs/kio_nfs.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/remote/kio_remote.h")));
    parsingUrls.append(
        QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/settings/kio_settings.cc")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/sftp/kio_sftp.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/smb/kio_smb.h")));
    // parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/tar/tar.h")));
    parsingUrls.append(
        QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/thumbnail/thumbnail.h")));
    parsingUrls.append(QUrl::fromLocalFile(kdebase + QLatin1String("/runtime/kioslave/trash/kio_trash.h")));

    out << "<hr />" << endl;
    out << "<h1>Analyzed KIO Slaves</h1>" << endl;
    out << "<ul>" << endl;
    foreach (const QUrl &url, parsingUrls) {
        out << "<li><a href='#" << url.fileName() << "'>" << url.path() << "</a></li>" << endl;
    }
    out << "</ul>" << endl;

    foreach (const QUrl &url, parsingUrls) {
        displayFoundMethods(existingMethods, url, out);
    }

    out << "<hr />" << endl;
    out << "<h1>SlaveBase methods to implement</h1>" << endl;
    out << "<ul>" << endl;
    foreach (const QString &method, existingMethods) {
        out << "<li>" << method << "</li>" << endl;
    }
    out << "</ul>" << endl;

    out << "</body>" << endl;
    out << "</html>" << endl;

    file.close();
}

void FindIncompatibleKioTest::displayFoundMethods(const QStringList &methods, const QUrl &url,
                                                  QTextStream &out)
{
    QMap<QString, bool> foundMethods = findMethods(methods, url);

    out << endl;
    out << "<hr />" << endl;
    out << "<h1 id='" << url.fileName() << "'>" << url.path() << "</h1>" << endl;

    if (foundMethods.isEmpty()) {
        out << "No file found" << endl;
        out << "<hr />" << endl;
        return;
    }

    out << "<hr />" << endl;
    out << "<h2><span class='found'>Found</span> and <span class='notFound'>Not Found</span> methods : </h2>"
        << endl;
    out << "<ul>" << endl;

    // Found methods
    QMapIterator<QString, bool> i(foundMethods);
    while (i.hasNext()) {
        i.next();

        if (i.value() == true) {
            out << "<li class='found'>" << i.key() << "</li>" << endl;
        }
    }

    // Found methods
    i = foundMethods;
    while (i.hasNext()) {
        i.next();

        if (i.value() == false) {
            out << "<li class='notFound'>" << i.key() << "</li>" << endl;
        }
    }

    out << "</ul>" << endl;
}

QMap<QString, bool> FindIncompatibleKioTest::findMethods(const QStringList &methods, const QUrl &url)
{
    QStringList lines = headerContent(url);
    if (lines.isEmpty())
        return QMap<QString, bool>();

    QMap<QString, bool> foundMethods;
    foreach (const QString &method, methods) {
        foundMethods.insert(method, false);
    }

    foreach (QString line, lines) {
        line = convertMethod(line);

        if (line.contains(QLatin1String("void")) == false)
            continue;

        // logDebug() << "Line" << convertMethod(line) << endl;

        foreach (const QString &method, methods) {
            if (line == convertMethod(method)) {
                foundMethods.insert(method, true);
            }
            /*
             else {
             logDebug() << method << " != " << line << endl;
             }
             */
        }
    }

    logDebug() << endl;

    return foundMethods;
}

QString FindIncompatibleKioTest::convertMethod(const QString &method)
{
    QString result(method);
    result = result.remove(QLatin1String("SlaveBase::"));
    result = result.remove(QLatin1String("virtual"));
    //	result = result.remove(QLatin1String(" "));
    //	result = result.replace('\t', ' ');
    result = result.remove(QLatin1String(";"));
    result = result.simplified();

    int firstParenthesis = result.indexOf(QLatin1String("("));
    int lastParenthesis = result.indexOf(QLatin1String(")"));
    if (firstParenthesis != -1 && lastParenthesis != -1)
        result = result.remove(firstParenthesis + 1, lastParenthesis - firstParenthesis - 1);

    return result;
}

QStringList FindIncompatibleKioTest::headerContent(const QUrl &url)
{
    QStringList kioHeaderContent;

    QString tmpFile;
    if (KIO::NetAccess::download(url, tmpFile, new QWidget())) {
        QFile file(tmpFile);
        file.open(QIODevice::ReadOnly);

        // If the file does not exist
        if (!file.exists()) {
            return QStringList();
        }

        QTextStream inputStream(&file);
        while (inputStream.atEnd() == false) {
            kioHeaderContent.append(inputStream.readLine());
        }

        file.close();

        KIO::NetAccess::removeTempFile(tmpFile);
    } else {
        logDebug() << KIO::NetAccess::lastErrorString() << endl;
        ;
    }

    return kioHeaderContent;
}

QTEST_KDEMAIN(FindIncompatibleKioTest, GUI)

#include "findIncompatibleKioTest.moc"
