#ifndef CDTABLEWIDGET_H
#define CDTABLEWIDGET_H

#include <QString>
#include <QTableWidget>

#include "CDPublic.h"

class CDTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    CDTableWidget(QWidget *parent = 0);
    virtual ~CDTableWidget();

public:
    void showUnequals(int showAsDir_1_Or_2, QList<Unequal> unequalList);

    void setAllItemTextColor(QColor c = Qt::gray);

signals:
    void cdToSubDir(QString subDirName);

private:
    void initConnections();

private slots:
    void slotItemDoubleClicked(QTableWidgetItem *pItem);

private:
    static const QStringList m_slHeaderLabels;

};

#endif // CDTABLEWIDGET_H
