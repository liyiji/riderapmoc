#ifndef CDMAINWINDOW_H
#define CDMAINWINDOW_H

#include <QDialog>

#include "CDPublic.h"

namespace Ui {
    class CDMainWindow;
}

class CDMainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CDMainWindow(QWidget *parent = 0);
    virtual ~CDMainWindow();

private:
    void initUi();
    void initConnections();

private slots:
    void slotChangeDir1();
    void slotChangeDir2();
    void slotStartCompare();

    void slotCdDir1SubDir(QString subDirName);
    void slotCdDir2SubDir(QString subDirName);
    void slotCdDir1ParentDir();
    void slotCdDir2ParentDir();
    void slotOpenDir1();
    void slotOpenDir2();

private:
    Ui::CDMainWindow *ui;
};

#endif // CDMAINWINDOW_H
