
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

    ui->pushButton->setEnabled(false);
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

    connect(ui->tableWidget, SIGNAL(cdToSubDir(QString)), this, SLOT(slotCdDir1SubDir(QString)));
    connect(ui->tableWidget_2, SIGNAL(cdToSubDir(QString)), this, SLOT(slotCdDir2SubDir(QString)));
}

void CDMainWindow::slotChangeDir1()
{
    QString dirName = QFileDialog::getExistingDirectory(this,
                                                        tr("Select Directory"),
                                                        ui->lineEdit->text());
    if (dirName.isEmpty() == false)
    {
        ui->lineEdit->setText(dirName);
        if (ui->lineEdit->text().isEmpty() == false && ui->lineEdit_2->text().isEmpty() == false)
        {
            ui->pushButton->setEnabled(true);
        }
    }
}

void CDMainWindow::slotChangeDir2()
{
    QString dirName = QFileDialog::getExistingDirectory(this,
                                                        tr("Select Directory"),
                                                        ui->lineEdit_2->text());
    if (dirName.isEmpty() == false)
    {
        ui->lineEdit_2->setText(dirName);
        if (ui->lineEdit->text().isEmpty() == false && ui->lineEdit_2->text().isEmpty() == false)
        {
            ui->pushButton->setEnabled(true);
        }
    }
}

void CDMainWindow::slotStartCompare()
{
    ui->label_3->setVisible(true);
    ui->label_4->setVisible(false);
    qApp->processEvents();

    QList<Unequal> unequalList;
    bool b = compareDir(ui->lineEdit->text(), ui->lineEdit_2->text(), unequalList);
    if (b)
    {
        ui->tableWidget->showUnequals(1, unequalList);
        ui->tableWidget_2->showUnequals(2, unequalList);
    }

    ui->label_3->setVisible(false);
    ui->label_4->setVisible(true);
}

void CDMainWindow::slotPause()
{
    /// TODO :
}

void CDMainWindow::slotCdDir1SubDir(QString subDirName)
{
    QString dir1Name = ui->lineEdit->text();
    QString fullSubDirName = dir1Name + QDir::separator() + subDirName;
    QDir dir(fullSubDirName);
    if (dir.exists())
    {
        ui->lineEdit->setText(fullSubDirName);
        ui->tableWidget->setAllItemTextColor(Qt::gray);
    }
}

void CDMainWindow::slotCdDir2SubDir(QString subDirName)
{
    QString dir2Name = ui->lineEdit_2->text();
    QString fullSubDirName = dir2Name + QDir::separator() + subDirName;
    QDir dir(fullSubDirName);
    if (dir.exists())
    {
        ui->lineEdit_2->setText(fullSubDirName);
        ui->tableWidget_2->setAllItemTextColor(Qt::gray);
    }
}
