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

#include "logViewFilterWidget.h"
#include "logViewWidget.h"
#include "logViewColumn.h"
#include "logging.h"
#include "logLevel.h"

#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QHBoxLayout>

#include <QComboBox>
#include <QStyledItemDelegate>
#include <QMetaEnum>
#include <QStandardItemModel>

#include <KComboBox>
#include <KLocalizedString>

class ComboBoxDelegate : public QStyledItemDelegate
{

public:
    explicit ComboBoxDelegate(QWidget *parent)
        : QStyledItemDelegate(parent)
    {
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem &refToNonConstOption = const_cast<QStyleOptionViewItem &>(option);
        refToNonConstOption.showDecorationSelected = false;
        QStyledItemDelegate::paint(painter, refToNonConstOption, index);
    }
};

class LogViewFilterWidgetPrivate
{
public:
    LogViewWidgetSearchLine *filterLine = nullptr;

    /**
     * Filter of the column list
     */
    KComboBox *filterList = nullptr;

    QComboBox *prioritiesComboBox = nullptr;

    QStandardItemModel *prioritiesModel = nullptr;
};

class LogViewWidgetSearchLinePrivate
{
public:
    bool priorities[Globals::LOG_LEVEL_NUM];

    LogViewWidgetSearchLinePrivate()
    {
        // Show all priorities.
        for (int i = 0; i < Globals::LOG_LEVEL_NUM; i++)
            priorities[i] = true;
    }
};

LogViewWidgetSearchLine::LogViewWidgetSearchLine()
    : KTreeWidgetSearchLine()
    , d(new LogViewWidgetSearchLinePrivate())
{
}

LogViewWidgetSearchLine::~LogViewWidgetSearchLine()
{
    delete d;
}

void LogViewWidgetSearchLine::updateSearch(const QString &pattern)
{
    KTreeWidgetSearchLine::updateSearch(pattern);

    Q_EMIT treeWidgetUpdated();
}

void LogViewWidgetSearchLine::setPriorityEnabled(int priority, bool enabled)
{
    if ((priority < 0) || (priority >= Globals::LOG_LEVEL_NUM))
        return;
    d->priorities[priority] = enabled;
    updateSearch(QString());
}

bool LogViewWidgetSearchLine::itemMatches(const QTreeWidgetItem *item, const QString &pattern) const
{
    // Hide item if its priority is not enabled.
    int priority = item->data(0, Qt::UserRole).toInt();
    if ((priority >= 0) && (priority < Globals::LOG_LEVEL_NUM)) {
        if (!d->priorities[priority])
            return false;
    }
    return KTreeWidgetSearchLine::itemMatches(item, pattern);
}

LogViewFilterWidget::LogViewFilterWidget()
    : d(new LogViewFilterWidgetPrivate())
{
    QHBoxLayout *filterBarLayout = new QHBoxLayout();
    filterBarLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(filterBarLayout);

    d->filterLine = new LogViewWidgetSearchLine();

    d->filterLine->setToolTip(i18n("Type your filter here"));
    d->filterLine->setWhatsThis(i18n("Allows you to only list items that match the content of this text."));
    d->filterLine->setPlaceholderText(i18n("Enter your search here..."));

    QLabel *filterIcon = new QLabel();
    filterIcon->setPixmap(QIcon::fromTheme(QStringLiteral("view-filter")).pixmap(style()->pixelMetric(QStyle::PM_SmallIconSize)));
    filterIcon->setBuddy(d->filterLine);
    filterBarLayout->addWidget(filterIcon);

    QLabel *filterLabel = new QLabel(i18n("Filter:"));
    filterLabel->setBuddy(d->filterLine);
    filterBarLayout->addWidget(filterLabel);

    filterBarLayout->addWidget(d->filterLine);

    initSearchListFilter();

    filterBarLayout->addWidget(d->filterList);

    d->prioritiesComboBox = new QComboBox(this);
    ComboBoxDelegate *delegate = new ComboBoxDelegate(d->prioritiesComboBox);
    d->prioritiesComboBox->setItemDelegate(delegate);
    filterBarLayout->addWidget(d->prioritiesComboBox);

    QMetaEnum &metaEnum = Globals::instance().logLevelsMetaEnum();

    d->prioritiesModel = new QStandardItemModel(d->prioritiesComboBox);
    d->prioritiesComboBox->setModel(d->prioritiesModel);

    QStandardItem *item = new QStandardItem(i18n("Select priorities"));
    item->setSelectable(false);
    d->prioritiesModel->appendRow(item);
    connect(d->prioritiesModel, &QStandardItemModel::itemChanged, this, &LogViewFilterWidget::prioritiesChanged);

    // Don't add last enum value into combobox.
    for (int i = 0; i < metaEnum.keyCount() - 1; i++) {
        int id = metaEnum.value(i);
        LogLevel *logLevel = Globals::instance().logLevelByPriority(id);

        QStandardItem *item = new QStandardItem(logLevel->name());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Checked, Qt::CheckStateRole);
        item->setData(metaEnum.value(i), Qt::UserRole);
        item->setData(QVariant(logLevel->color()), Qt::ForegroundRole);

        d->prioritiesModel->appendRow(item);
    }
}

LogViewFilterWidget::~LogViewFilterWidget()
{
    delete d;
}

void LogViewFilterWidget::initSearchListFilter()
{
    d->filterList = new KComboBox();

    d->filterList->setToolTip(i18n("Choose the filtered column here"));
    d->filterList->setWhatsThis(i18n(
        "Allows you to apply the item filter only on the specified column here. \"<i>All</i>\" column means "
        "no specific filter."));

    d->filterList->addItem(i18n("All"));

    d->filterList->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    connect(d->filterList, SIGNAL(activated(int)), d->filterLine, SLOT(setFocus()));
    connect(d->filterList, QOverload<int>::of(&KComboBox::activated), this, &LogViewFilterWidget::changeColumnFilter);
    connect(d->filterList, SIGNAL(activated(int)), d->filterLine, SLOT(updateSearch()));
}

void LogViewFilterWidget::updateFilterColumns(const LogViewColumns &columns)
{
    logDebug() << "Changing columns...";

    // We first delete all items
    d->filterList->clear();

    // Then we insert the default items
    d->filterList->addItem(i18n("All"));

    foreach (const LogViewColumn &column, columns.columns()) {
        if (column.isFiltered() == true) {
            d->filterList->addItem(column.columnName());
        }
    }

    d->filterList->setCurrentIndex(0);
}

void LogViewFilterWidget::changeColumnFilter(int column)
{
    // The user select all columns
    if (column == 0) {
        logDebug() << "Searching on all columns";

        d->filterLine->setSearchColumns(QList<int>());
        return;
    }

    logDebug() << "Searching on " << d->filterList->currentIndex() << " column";

    QList<int> filterColumns;
    // currentIndex() - 1 to do not count the "All" columns item
    filterColumns.append(d->filterList->currentIndex() - 1);

    d->filterLine->setSearchColumns(filterColumns);
}

void LogViewFilterWidget::prioritiesChanged(QStandardItem *item)
{
    int priority = item->data(Qt::UserRole).toInt();
    bool priorityEnabled = (item->checkState() == Qt::Checked);
    d->filterLine->setPriorityEnabled(priority, priorityEnabled);
    if (priorityEnabled)
        logDebug() << "Show entries with priority" << priority;
    else
        logDebug() << "Hide entries with priority" << priority;
}

KComboBox *LogViewFilterWidget::filterList()
{
    return d->filterList;
}

LogViewWidgetSearchLine *LogViewFilterWidget::filterLine()
{
    return d->filterLine;
}
