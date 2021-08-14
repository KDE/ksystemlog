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

#include "levelPrintPage.h"

#include <KLocalizedString>

#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>

#include "logLevel.h"
#include "logViewWidgetItem.h"
#include "logging.h"

#include "globals.h"

LevelPrintPage::LevelPrintPage(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(i18n("Log Level Printing"));

    mPageLayout = new QVBoxLayout(this);

    mLblChoose = new QLabel(this);
    // m_lblChoose->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0,
    // m_lblChoose->sizePolicy().hasHeightForWidth() ) );
    mLblChoose->setText(i18n("Choose which log levels you wish to print in color."));
    mPageLayout->addWidget(mLblChoose);

    mBtnGroup = new QButtonGroup(this);
    /*
    i18n("Log Levels"),
    m_btnGroup->setColumnLayout(0, Qt::Vertical );
    m_btnGroup->layout()->setSpacing( 6 );
    m_btnGroup->layout()->setMargin( 11 );
    m_btnGroupLayout = new QGridLayout( m_btnGroup->layout() );
    m_btnGroupLayout->setAlignment( Qt::AlignTop );
    */

    int row = 0;
    int col = 0;
    const auto logLevels = Globals::instance().logLevels();
    mLevelCheckBoxes.reserve(logLevels.count());
    for (LogLevel *level : logLevels) {
        auto button = new QCheckBox(level->name(), this); //, m_btnGroup, 0

        mLevelCheckBoxes.append(button);
        mBtnGroup->addButton(button, level->id());
        mBtnGroupLayout->addWidget(button, row, col);

        logDebug() << "name: " << level->name() << " id: " << level->id();

        row++;
        if (row >= 4) {
            row = 0;
            col++;
        }
    }

    // m_pageLayout->addWidget(m_btnGroup);
}

LevelPrintPage::~LevelPrintPage()
{
    // no need to delete child widgets, Qt does it all for us
}

/* QPrinter Port: comment out as dialog page is not currently being used, so not ported

void LevelPrintPage::getOptions( QMap<QString,QString>& opts, bool incldef ) {
    foreach(LogLevel* level, Globals::instance().logLevels()) {
        QString key = "kde-ksystemlog-print_" + level->name();


        QCheckBox* checkBox = static_cast<QCheckBox*>(m_btnGroup->find(level->id()));
        if(checkBox) {
            if (checkBox->isChecked())
                opts[ key ] = "1";
            else
                opts[ key ] = "0";
        }

    }
}

void LevelPrintPage::setOptions( const QMap<QString,QString>& opts ) {
    foreach(LogLevel* level, Globals::instance().logLevels()) {
        QString key = "kde-ksystemlog-print_" + level->name();
        QString use = opts[ key ];

        int chk = use.toInt();


        QCheckBox* checkBox = static_cast<QCheckBox*>(m_btnGroup->find(level->id()));
        if(checkBox) {
            if(chk)
                checkBox->setChecked(true);
            else
                checkBox->setChecked(false);
        }


    }
}

*/

bool LevelPrintPage::isValid(QString & /*msg*/)
{
    return true;
}
