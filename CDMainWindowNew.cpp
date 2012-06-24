
#include "CDMainWindowNew.h"
#include "ui_CDMainWindowNew.h"

#include <QFileDialog>
#include <QProcess>
#include <QScrollBar>

CDMainWindowNew::CDMainWindowNew(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::CDMainWindowNew)
{
    ui->setupUi(this);
    initUi();
    initConnections();
}

CDMainWindowNew::~CDMainWindowNew()
{
    delete ui;
}

void CDMainWindowNew::initUi()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();

    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);

    ui->comboBox_3->addItem("D:\\CacheData");
    ui->comboBox_3->addItem("E:\\Data1");
    ui->comboBox_3->addItem("F:\\Data2");
    ui->comboBox_4->addItem("L:\\Backup-2012-06-08-CacheData");
    ui->comboBox_4->addItem("K:\\Backup-2012-06-08-Data1");
    ui->comboBox_4->addItem("L:\\Backup-2012-06-08-Data2");

    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->comboBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->comboBox_3->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->comboBox_4->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    bool bDebug = false;
    if (bDebug)
    {
        ui->lineEdit->setText("D:\\SkyDrive");
        ui->lineEdit_2->setText("G:\\Ftp.Folder.For.Xlight");
    }
}

void CDMainWindowNew::initConnections()
{
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(slotChangeDir1()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(slotChangeDir2()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotStartCompare()));

    connect(ui->tableWidget, SIGNAL(cdToSubDir(QString)), this, SLOT(slotCdDir1SubDir(QString)));
    connect(ui->tableWidget_2, SIGNAL(cdToSubDir(QString)), this, SLOT(slotCdDir2SubDir(QString)));

    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotCdDir1ParentDir()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(slotCdDir2ParentDir()));

    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(slotOpenDir1()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(slotOpenDir2()));

    connect(ui->pushButton_6, SIGNAL(clicked()), ui->tableWidget, SLOT(slotResizeTable()));
    connect(ui->pushButton_6, SIGNAL(clicked()), ui->tableWidget_2, SLOT(slotResizeTable()));

    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(slotCdDir1SubDir()));
    connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(slotCdDir2SubDir()));

    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotUpdateDir1SubDirNames(QString)));
    connect(ui->lineEdit_2, SIGNAL(textChanged(QString)), this, SLOT(slotUpdateDir2SubDirNames(QString)));

    connect(ui->pushButton_11, SIGNAL(clicked()), this, SLOT(slotCdCommonDirToDir1()));
    connect(ui->pushButton_12, SIGNAL(clicked()), this, SLOT(slotCdCommonDirToDir2()));

    connect(ui->checkBox, SIGNAL(toggled(bool)), this, SLOT(slotSyncScroll(bool)));
    ui->checkBox->setChecked(true);

    connect(ui->checkBox_2, SIGNAL(toggled(bool)), this, SLOT(slotSyncSelection(bool)));
    ui->checkBox_2->setChecked(true);
}

QStringList CDMainWindowNew::getSubDirNames(QString dirName)
{
    QStringList subDirNames;
    QDir dir(dirName);
    if (dir.exists())
    {
        subDirNames = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Name);
    }

    return subDirNames;
}

void CDMainWindowNew::slotChangeDir1()
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

void CDMainWindowNew::slotChangeDir2()
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

void CDMainWindowNew::slotStartCompare()
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

void CDMainWindowNew::slotCdDir1SubDir()
{
    QString subDirName = ui->comboBox->currentText();
    if (subDirName.isEmpty() == false)
    {
        slotCdDir1SubDir(subDirName);
    }
}

void CDMainWindowNew::slotCdDir2SubDir()
{
    QString subDirName = ui->comboBox_2->currentText();
    if (subDirName.isEmpty() == false)
    {
        slotCdDir2SubDir(subDirName);
    }
}

void CDMainWindowNew::slotCdDir1SubDir(QString subDirName)
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

void CDMainWindowNew::slotCdDir2SubDir(QString subDirName)
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

void CDMainWindowNew::slotCdDir1ParentDir()
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

void CDMainWindowNew::slotCdDir2ParentDir()
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

void CDMainWindowNew::slotOpenDir1()
{
    QDir dir(ui->lineEdit->text());
    if (dir.exists())
    {
#ifdef WIN32
        QString path = ui->lineEdit->text();
        path.replace("/","\\");
        QProcess::startDetached("explorer " + path);
#endif
    }
}

void CDMainWindowNew::slotOpenDir2()
{
    QDir dir(ui->lineEdit_2->text());
    if (dir.exists())
    {
#ifdef WIN32
        QString path = ui->lineEdit_2->text();
        path.replace("/","\\");
        QProcess::startDetached("explorer " + path);
#endif
    }
}

void CDMainWindowNew::slotUpdateDir1SubDirNames(QString dir1NewName)
{
    ui->comboBox->clear();
    ui->comboBox->addItems(getSubDirNames(dir1NewName));
}

void CDMainWindowNew::slotUpdateDir2SubDirNames(QString dir2NewName)
{
    ui->comboBox_2->clear();
    ui->comboBox_2->addItems(getSubDirNames(dir2NewName));
}

void CDMainWindowNew::slotCdCommonDirToDir1()
{
    QString commonDir = ui->comboBox_3->currentText();
    QDir dir(commonDir);
    if (dir.exists())
    {
        ui->lineEdit->setText(commonDir);
    }
}

void CDMainWindowNew::slotCdCommonDirToDir2()
{
    QString commonDir = ui->comboBox_4->currentText();
    QDir dir(commonDir);
    if (dir.exists())
    {
        ui->lineEdit_2->setText(commonDir);
    }
}

void CDMainWindowNew::slotSyncScroll(bool b)
{
    if (b)
    {
        connect(ui->tableWidget->verticalScrollBar(), SIGNAL(valueChanged(int)),
                ui->tableWidget_2->verticalScrollBar(), SLOT(setValue(int)));
        connect(ui->tableWidget_2->verticalScrollBar(), SIGNAL(valueChanged(int)),
                ui->tableWidget->verticalScrollBar(), SLOT(setValue(int)));
    }
    else
    {
        disconnect(ui->tableWidget->verticalScrollBar(), SIGNAL(valueChanged(int)),
                   ui->tableWidget_2->verticalScrollBar(), SLOT(setValue(int)));
        disconnect(ui->tableWidget_2->verticalScrollBar(), SIGNAL(valueChanged(int)),
                   ui->tableWidget->verticalScrollBar(), SLOT(setValue(int)));
    }
}

void CDMainWindowNew::slotSyncSelection(bool b)
{
    if (b)
    {
        connect(ui->tableWidget, SIGNAL(itemSelectionChanged(int, int)),
                ui->tableWidget_2, SLOT(slotSelectItem(int,int)));
        connect(ui->tableWidget_2, SIGNAL(itemSelectionChanged(int, int)),
                ui->tableWidget, SLOT(slotSelectItem(int,int)));
    }
    else
    {
        disconnect(ui->tableWidget, SIGNAL(itemSelectionChanged(int, int)),
                   ui->tableWidget_2, SLOT(slotSelectItem(int,int)));
        disconnect(ui->tableWidget_2, SIGNAL(itemSelectionChanged(int, int)),
                   ui->tableWidget, SLOT(slotSelectItem(int,int)));
    }
}
