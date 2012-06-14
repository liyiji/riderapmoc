#ifndef CDMAINWINDOW_H
#define CDMAINWINDOW_H

#include <QDialog>

#include <CDPublic.h>

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

    void showUnequals(QList<Unequal> unequalList);

private slots:
    void slotChangeDir1();
    void slotChangeDir2();
    void slotStartCompare();
    void slotPause();

private:
    Ui::CDMainWindow *ui;
};

#endif // CDMAINWINDOW_H
