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

signals:
    void cdToSubDir(QString subDirName);

public:
    void showUnequals(int showAsDir_1_Or_2, QList<Unequal> unequalList);
    void setAllItemTextColorToGray();
    void setAllItemTextColor(QColor c = Qt::gray);

public slots:
    void resizeTable();

private:
    void initConnections();
    void ChangeIntoSubDirByItem(QTableWidgetItem *pItem);

    virtual void contextMenuEvent(QContextMenuEvent *e);

private slots:
    void slotEmitCdToSubDirOfCurrentItem();

private:
    static const QStringList m_slHeaderLabels;

};

#endif // CDTABLEWIDGET_H
