
#ifndef CDMAINWINDOWNEW_H
#define CDMAINWINDOWNEW_H

#include <QMainWindow>

#include "CDPublic.h"

class QTableWidgetItem;

namespace Ui {
    class CDMainWindowNew;
}

class CDMainWindowNew : public QMainWindow
{
    Q_OBJECT

public:
    explicit CDMainWindowNew(QWidget *parent = 0);
    virtual ~CDMainWindowNew();

private:
    void initUi();
    void initConnections();
    QStringList getSubDirNames(QString dirName);

private slots:
    void slotChangeDir1();
    void slotChangeDir2();
    void slotStartCompare();

    void slotCdDir1SubDir();
    void slotCdDir2SubDir();
    void slotCdDir1SubDir(QString subDirName);
    void slotCdDir2SubDir(QString subDirName);
    void slotCdDir1ParentDir();
    void slotCdDir2ParentDir();
    void slotOpenDir1();
    void slotOpenDir2();
    void slotUpdateDir1SubDirNames(QString dir1NewName);
    void slotUpdateDir2SubDirNames(QString dir2NewName);
    void slotCdCommonDirToDir1();
    void slotCdCommonDirToDir2();

    void slotSyncScroll(bool b);
    void slotSyncSelection(bool b);

private:
    Ui::CDMainWindowNew *ui;
};

#endif // CDMAINWINDOWNEW_H
