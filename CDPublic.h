
#ifndef CDPUBLIC_H
#define CDPUBLIC_H

#include <QDir>
#include <QDebug>

enum EntryType
{
    FOLDERS,
    FILES
};

enum UnequalType
{
    ONLY_IN_DIR_1 = 1,
    ONLY_IN_DIR_2 = 2,
    SIZE_UNEQUAL,
    DETAIL_DIFFERENT
};

struct Unequal
{
    QString     m_sDir1;
    QString     m_sDir2;
    QString     m_sFileOrDirName;
    EntryType   m_eEntryType;
    UnequalType m_eUnequalType;
    qint64      m_iSize1;
    qint64      m_iSize2;
};

void debugUnequal(Unequal ueq);

bool compareDir(QString strDir1, QString strDir2,
                QList<Unequal> &unequalList);
bool compareFiles(QDir dir1, QDir dir2,
                  QStringList fileList1, QStringList fileList2,
                  QList<Unequal> &unequalList);

qint64 calculateDirSize(QString dirName);

void separateDiffences(QStringList stl1, QStringList stl2,
                       QStringList &stl1Only, QStringList &stl2Only, QStringList &stlCommon);

QString getScientificNotationOfQInt64(qint64 num);

#endif // CDPUBLIC_H
