
#include "CDMainWindow.h"
#include "ui_CDMainWindow.h"

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

    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);

    bool bDebug = true;
    if (bDebug)
    {
        ui->lineEdit->setText("D:\\SkyDrive");
        ui->lineEdit_2->setText("G:\\Ftp.Folder.For.Xlight");
    }
}

void CDMainWindow::initConnections()
{
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(slotChangeDir1()));
    connect(ui->toolButton_2, SIGNAL(clicked()), this, SLOT(slotChangeDir2()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotStartCompare()));

    connect(ui->tableWidget, SIGNAL(cdToSubDir(QString)), this, SLOT(slotCdDir1SubDir(QString)));
    connect(ui->tableWidget_2, SIGNAL(cdToSubDir(QString)), this, SLOT(slotCdDir2SubDir(QString)));

    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotCdDir1ParentDir()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(slotCdDir2ParentDir()));
}

void CDMainWindow::slotChangeDir1()
{
    QString dirName = QFileDialog::getExistingDirectory(this,
                                                        tr("Select Directory"),
                                                        ui->lineEdit->text());
    if (dirName.isEmpty() == false)
    {
        QString oldDirName = ui->lineEdit->text();
        if (oldDirName != dirName)
        {
            ui->tableWidget->setAllItemTextColorToGray();
        }
        ui->lineEdit->setText(dirName);
        if (ui->lineEdit->text().isEmpty() == false)
        {
            ui->pushButton_2->setEnabled(true);
            if (ui->lineEdit_2->text().isEmpty() == false)
            {
                ui->pushButton->setEnabled(true);
            }
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
        QString oldDirName = ui->lineEdit_2->text();
        if (oldDirName != dirName)
        {
            ui->tableWidget_2->setAllItemTextColorToGray();
        }
        ui->lineEdit_2->setText(dirName);
        if (ui->lineEdit_2->text().isEmpty() == false)
        {
            ui->pushButton_4->setEnabled(true);
            if (ui->lineEdit->text().isEmpty() == false)
            {
                ui->pushButton->setEnabled(true);
            }
        }
    }
}

void CDMainWindow::slotStartCompare()
{
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty())
    {
        return;
    }

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

void CDMainWindow::slotCdDir1SubDir(QString subDirName)
{
    QString dir1Name = ui->lineEdit->text();
    QString fullSubDirName = dir1Name + QDir::separator() + subDirName;
    QDir dir(fullSubDirName);
    if (dir.exists())
    {
        ui->lineEdit->setText(fullSubDirName);
        ui->tableWidget->setAllItemTextColorToGray();
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
        ui->tableWidget_2->setAllItemTextColorToGray();
    }
}

void CDMainWindow::slotCdDir1ParentDir()
{
    if (ui->lineEdit->text().isEmpty())
    {
        return;
    }
    QDir dir(ui->lineEdit->text());
    dir.cdUp();
    ui->lineEdit->setText(dir.absolutePath());
    ui->tableWidget->setAllItemTextColorToGray();
}

void CDMainWindow::slotCdDir2ParentDir()
{
    if (ui->lineEdit_2->text().isEmpty())
    {
        return;
    }
    QDir dir(ui->lineEdit_2->text());
    dir.cdUp();
    ui->lineEdit_2->setText(dir.absolutePath());
    ui->tableWidget_2->setAllItemTextColorToGray();
}
