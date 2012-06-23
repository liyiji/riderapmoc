
#include "CDTableWidget.h"

#include <QApplication>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QMenu>
#include <QScrollBar>

/**
  Folder / File     /// 文件夹 / 文件
  Status            /// 状态，Surplus / Lack / Different Size
  Name              /// 文件夹 / 文件 的名字
  Size              /// 文件夹 / 文件 的大小
  */
const QStringList CDTableWidget::m_slHeaderLabels = (QStringList()
                                                     << "Folder / File"
                                                     << "Status"
                                                     << "Name"
                                                     << "Size");

CDTableWidget::CDTableWidget(QWidget *parent) : QTableWidget(parent)
{
    setRowCount(0);

    setColumnCount(m_slHeaderLabels.size());
    setHorizontalHeaderLabels(m_slHeaderLabels);

    /// 禁止编辑
    setEditTriggers(QTableWidget::NoEditTriggers);

    /// 横着的表头不许移动
    horizontalHeader()->setMovable(false);

    /// 竖着的表头可见
    verticalHeader()->setVisible(true);

    /// 横着的表头点击排序
    setSortingEnabled(true);

    /// 按行选中
    setSelectionBehavior(QTableWidget::SelectRows);

    /// 只能单选
    setSelectionMode(QAbstractItemView::SingleSelection);

    setGridStyle(Qt::NoPen);

    /// 每列宽度、每行高度自动设置为合适的大小
    slotResizeTable();

    initConnections();
}

CDTableWidget::~CDTableWidget()
{

}

void CDTableWidget::showUnequals(int showAsDir_1_Or_2, QList<Unequal> unequalList)
{
    setRowCount(0);
    if (showAsDir_1_Or_2 != 1 && showAsDir_1_Or_2 != 2)
    {
        return;
    }

    for (int i = 0; i < unequalList.size(); i++)
    {
        Unequal ueq = unequalList[i];
        QTableWidgetItem **pItem = new QTableWidgetItem *[m_slHeaderLabels.size()];
        for (int j = 0; j < m_slHeaderLabels.size(); j++)
        {
            pItem[j] = new QTableWidgetItem();
        }

        switch (ueq.m_eEntryType)
        {
        case FOLDERS:
            {
                pItem[0]->setText("Folder");
                pItem[0]->setTextColor(Qt::darkBlue);
                break;
            }
        case FILES:
            {
                pItem[0]->setText("File");
                break;
            }
        default:
            {
                pItem[0]->setText("Unknown");
                break;
            }
        }

        if (ueq.m_eUnequalType == SIZE_UNEQUAL)
        {
            pItem[1]->setText("Different Size");
            pItem[1]->setTextColor(Qt::darkYellow);
        }
        else
        {
            if (showAsDir_1_Or_2 == ueq.m_eUnequalType)
            {
                pItem[1]->setText("Surplus");
                pItem[1]->setTextColor(Qt::darkGreen);
            }
            else
            {
                pItem[1]->setText("Lack");
                pItem[1]->setTextColor(Qt::darkRed);
            }
        }

        pItem[2]->setText(ueq.m_sFileOrDirName);

        switch (showAsDir_1_Or_2)
        {
        case 1:
            {
                pItem[3]->setText(getScientificNotationOfQInt64(ueq.m_iSize1));
                break;
            }
        case 2:
            {
                pItem[3]->setText(getScientificNotationOfQInt64(ueq.m_iSize2));
                break;
            }
        default:
            {
                break;
            }
        }
        pItem[3]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        insertRow(0);
        for (int iColumn = 0; iColumn < m_slHeaderLabels.size(); iColumn++)
        {
            setItem(0, iColumn, pItem[iColumn]);
        }

        delete []pItem;
    }

    slotResizeTable();
    slotSetDifferentBackgroundColorBetweenNearRow();
}

void CDTableWidget::setAllItemTextColorToGray()
{
    setAllItemTextColor(Qt::gray);
}

void CDTableWidget::setAllItemTextColor(QColor c)
{
    for (int i = 0; i < rowCount(); i++)
    {
        for (int j = 0; j < columnCount(); j++)
        {
            QTableWidgetItem *pItem = item(i, j);
            if (pItem)
            {
                pItem->setTextColor(c);
            }
        }
    }
}

void CDTableWidget::slotResizeTable()
{
    int pos = verticalScrollBar()->value();

    /// 先调整宽度
    QVector<int> widths;
    widths.resize(m_slHeaderLabels.size());
    widths.fill(-1);
    for (int i = 0; i < rowCount(); i++)
    {
        QTableWidgetItem *pItem = item(i, 0);
        setCurrentItem(pItem);
        resizeColumnsToContents();
        for (int j = 0; j < m_slHeaderLabels.size(); j++)
        {
            if (widths[j] <= columnWidth(j))
            {
                widths[j] = columnWidth(j);
            }
        }
    }

    for (int i = 0; i < columnCount(); i++)
    {
        setColumnWidth(i, widths[i]);
    }

    /// 再调整高度
    resizeRowsToContents();

    /// 最后设定显示的范围
    setCurrentItem(item(0, 0));
    setCurrentItem(item(-1, -1));

    verticalScrollBar()->setValue(pos);
}

void CDTableWidget::slotSetDifferentBackgroundColorBetweenNearRow()
{
    QColor lightLightGray(240, 240, 240);
    for (int i = 0; i < rowCount(); i++)
    {
        if (i % 2 != 0)
        {
            for (int j = 0; j < columnCount(); j++)
            {
                QTableWidgetItem *pItem = item(i, j);
                if (pItem)
                {
                    pItem->setBackgroundColor(lightLightGray);
                }
            }
        }
    }
}

void CDTableWidget::slotSelectItem(int iRow, int iColumn)
{
    setCurrentItem(item(iRow, iColumn));
}

void CDTableWidget::initConnections()
{
    connect(this, SIGNAL(itemSelectionChanged()),
            this, SLOT(slotItemSelectionChanged()));
}

void CDTableWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QTableWidgetItem *pItem = itemAt(e->pos());

    if (pItem)
    {
        int iRow = pItem->row();
        QStringList itemTexts;
        for (int iColumn = 0; iColumn < columnCount(); iColumn++)
        {
            itemTexts.append(item(iRow, iColumn)->text());
        }
        if (itemTexts[0] != "Folder")
        {
            return;
        }

        QMenu menu(this);
        menu.addAction("Change into this folder", this, SLOT(slotEmitCdToSubDirOfCurrentItem()));
        menu.exec(QCursor::pos());
    }
}

void CDTableWidget::paintEvent(QPaintEvent *e)
{
    /// 不能轻易往这里添加东西啊，否则可能会出问题哦
    QTableWidget::paintEvent(e);
}

void CDTableWidget::ChangeIntoSubDirByItem(QTableWidgetItem *pItem)
{
    if (pItem == 0)
    {
        return;
    }

    int iRow = pItem->row();

    QStringList itemTexts;
    for (int iColumn = 0; iColumn < columnCount(); iColumn++)
    {
        itemTexts.append(item(iRow, iColumn)->text());
    }
    if (itemTexts[0] != "Folder")
    {
        return;
    }

    QString subDirName = itemTexts[2];
    emit cdToSubDir(subDirName);
}

void CDTableWidget::slotEmitCdToSubDirOfCurrentItem()
{
    QTableWidgetItem *pItem = currentItem();
    if (pItem)
    {
        ChangeIntoSubDirByItem(pItem);
    }
}

void CDTableWidget::slotItemSelectionChanged()
{
    QTableWidgetItem *pItem = currentItem();
    if (pItem)
    {
        emit itemSelectionChanged(pItem->row(), pItem->column());
    }
}
