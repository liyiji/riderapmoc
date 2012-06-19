
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
    void verticalScrollBarMoved(int pos);
    void itemSelectionChanged(int iRow, int iColumn);

public:
    void showUnequals(int showAsDir_1_Or_2, QList<Unequal> unequalList);
    void setAllItemTextColorToGray();
    void setAllItemTextColor(QColor c = Qt::gray);

public slots:
    void slotResizeTable();
    void slotSetDifferentBackgroundColorBetweenNearRow();
    void slotSelectItem(int iRow, int iColumn = 0);

private:
    void initConnections();
    void ChangeIntoSubDirByItem(QTableWidgetItem *pItem);

    virtual void contextMenuEvent(QContextMenuEvent *e);
    virtual void paintEvent(QPaintEvent *e);

private slots:
    void slotEmitCdToSubDirOfCurrentItem();
    void slotItemSelectionChanged();

private:
    static const QStringList m_slHeaderLabels;

};

#endif // CDTABLEWIDGET_H
