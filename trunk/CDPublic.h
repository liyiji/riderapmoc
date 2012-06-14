
#ifndef CDPUBLIC_H
#define CDPUBLIC_H

#include <QDir>
#include <QDebug>

enum EntryType
{
    DIRECTORIES,
    FILES
};

enum UnequalType
{
    SIZE_UNEQUAL,
    NOT_IN_DIR_1,
    NOT_IN_DIR_2
};

struct Unequal
{
    QString     m_sDir1;
    QString     m_sDir2;
    QString     m_sFileOrDirName;
    EntryType   m_eEntryType;
    UnequalType m_eUnequalType;
};

bool compareDir(QString strDir1, QString strDir2,
                QList<Unequal> &unequalList);
bool compareFiles(QDir dir1, QDir dir2,
                  QStringList fileList1, QStringList fileList2,
                  QList<Unequal> &unequalList);

void debugUnequal(Unequal ueq);

qint64 calculateDirSize(QString dirName);

void separateDiffences(QStringList stl1, QStringList stl2,
                       QStringList &stl1Only, QStringList &stl2Only, QStringList &stlCommon);

#endif // CDPUBLIC_H
