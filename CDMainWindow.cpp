
#include "CDMainWindow.h"
#include "ui_CDMainWindow.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>

CDMainWindow::CDMainWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CDMainWindow)
{
    ui->setupUi(this);
    initUi();
    initConnections();
}

CDMainWindow::~CDMainWindow()
{
    delete ui;
}

void CDMainWindow::initUi()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();

    /// 处理tableWidget
    {
        /// 禁止编辑
        ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
        ui->tableWidget_2->setEditTriggers(QTableWidget::NoEditTriggers);

        QHeaderView *headerView[2];
        headerView[0] = ui->tableWidget->horizontalHeader();
        headerView[1] = ui->tableWidget_2->horizontalHeader();
        for (int i = 0; i < 2; i++)
        {
            headerView[i]->setMovable(false);
        }

        /// 每列宽度、每行高度自动设置为合适的大小
        /// TODO :
//        resizeTableWidgetAccount();
//        resizeTableWidgetIncomeExpenseLiquidity();

        /// 竖着的表头可见
        ui->tableWidget->verticalHeader()->setVisible(true);
        ui->tableWidget_2->verticalHeader()->setVisible(true);

        /// 横着的表头点击排序
        ui->tableWidget->setSortingEnabled(true);
        ui->tableWidget_2->setSortingEnabled(true);

        /// 按行选中
        ui->tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
        ui->tableWidget_2->setSelectionBehavior(QTableWidget::SelectRows);

        /// 只能单选
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    }

    ui->pushButton_2->setEnabled(false);

    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
}

void CDMainWindow::initConnections()
{
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(slotChangeDir1()));
    connect(ui->toolButton_2, SIGNAL(clicked()), this, SLOT(slotChangeDir2()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotStartCompare()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotPause()));
}

void CDMainWindow::showUnequals(QList<Unequal> unequalList)
{
    ui->tableWidget->clear();
    ui->tableWidget_2->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);

    QStringList stl;
    stl.append("Status");
    stl.append("File or Dir");
    stl.append("Name");
    ui->tableWidget->setColumnCount(stl.size());
    ui->tableWidget->setHorizontalHeaderLabels(stl);
    ui->tableWidget_2->setColumnCount(stl.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(stl);

    int ithRow1 = 0;
    int ithRow2 = 0;
    for (int i = 0; i < unequalList.size(); i++)
    {
        Unequal ueq = unequalList[i];
        int ithColume = 0;

        switch (ueq.m_eUnequalType)
        {
        case SIZE_UNEQUAL:
        {
            ui->tableWidget->insertRow(0);
            ui->tableWidget_2->insertRow(0);

            QTableWidgetItem *pItem01 = new QTableWidgetItem("Different Size");
            QTableWidgetItem *pItem02 = new QTableWidgetItem("Different Size");
            ui->tableWidget->setItem(ithRow1, ithColume, pItem01);
            ui->tableWidget_2->setItem(ithRow2, ithColume, pItem02);
            ithColume++;

            switch (ueq.m_eEntryType)
            {
            case DIRECTORIES:
            {
                QTableWidgetItem *pItem11 = new QTableWidgetItem("Directory");
                QTableWidgetItem *pItem21 = new QTableWidgetItem("Directory");
                ui->tableWidget->setItem(ithRow1, ithColume, pItem11);
                ui->tableWidget_2->setItem(ithRow2, ithColume, pItem21);

                break;
            }
            case FILES:
            {
                QTableWidgetItem *pItem11 = new QTableWidgetItem("File");
                QTableWidgetItem *pItem21 = new QTableWidgetItem("File");
                ui->tableWidget->setItem(ithRow1, ithColume, pItem11);
                ui->tableWidget_2->setItem(ithRow2, ithColume, pItem21);

                break;
            }
            default:
            {
                break;
            }
            }
            ithColume++;

            QTableWidgetItem *pItem21 = new QTableWidgetItem(ueq.m_sFileOrDirName);
            QTableWidgetItem *pItem22 = new QTableWidgetItem(ueq.m_sFileOrDirName);
            ui->tableWidget->setItem(ithRow1, ithColume, pItem21);
            ui->tableWidget_2->setItem(ithRow2, ithColume, pItem22);

            break;
        }
        case NOT_IN_DIR_1:
        {
            ui->tableWidget_2->insertRow(0);

            QTableWidgetItem *pItem0 = new QTableWidgetItem("Surplus");
            ui->tableWidget_2->setItem(ithRow2, ithColume, pItem0);
            ithColume++;

            switch (ueq.m_eEntryType)
            {
            case DIRECTORIES:
            {
                QTableWidgetItem *pItem1 = new QTableWidgetItem("Directory");
                ui->tableWidget_2->setItem(ithRow2, ithColume, pItem1);

                break;
            }
            case FILES:
            {
                QTableWidgetItem *pItem1 = new QTableWidgetItem("File");
                ui->tableWidget_2->setItem(ithRow2, ithColume, pItem1);

                break;
            }
            default:
            {
                break;
            }
            }
            ithColume++;

            QTableWidgetItem *pItem2 = new QTableWidgetItem(ueq.m_sFileOrDirName);
            ui->tableWidget_2->setItem(ithRow2, ithColume, pItem2);

            break;
        }
        case NOT_IN_DIR_2:
        {
            ui->tableWidget->insertRow(0);

            QTableWidgetItem *pItem0 = new QTableWidgetItem("Surplus");
            ui->tableWidget->setItem(ithRow1, ithColume, pItem0);
            ithColume++;

            switch (ueq.m_eEntryType)
            {
            case DIRECTORIES:
            {
                QTableWidgetItem *pItem1 = new QTableWidgetItem("Directory");
                ui->tableWidget->setItem(ithRow1, ithColume, pItem1);

                break;
            }
            case FILES:
            {
                QTableWidgetItem *pItem1 = new QTableWidgetItem("File");
                ui->tableWidget->setItem(ithRow1, ithColume, pItem1);

                break;
            }
            default:
            {
                break;
            }
            }
            ithColume++;

            QTableWidgetItem *pItem2 = new QTableWidgetItem(ueq.m_sFileOrDirName);
            ui->tableWidget->setItem(ithRow1, ithColume, pItem2);

            break;
        }
        default:
        {
            break;
        }
        }
    }
}

void CDMainWindow::slotChangeDir1()
{
    QString dirName = QFileDialog::getExistingDirectory();
    if (dirName.isEmpty() == false)
    {
        ui->lineEdit->setText(dirName);
    }
}

void CDMainWindow::slotChangeDir2()
{
    QString dirName = QFileDialog::getExistingDirectory();
    if (dirName.isEmpty() == false)
    {
        ui->lineEdit_2->setText(dirName);
    }
}

void CDMainWindow::slotStartCompare()
{
    ui->label_3->setVisible(true);
    ui->label_4->setVisible(false);

    QList<Unequal> unequalList;
    bool b = compareDir(ui->lineEdit->text(), ui->lineEdit_2->text(), unequalList);
    if (b)
    {
        showUnequals(unequalList);
    }
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(true);
}

void CDMainWindow::slotPause()
{

}
