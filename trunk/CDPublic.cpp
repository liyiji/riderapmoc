
#include "CDPublic.h"

#include <QString>

void debugUnequal(Unequal ueq)
{
    qDebug() << ueq.m_sDir1 << ueq.m_sDir2 << ueq.m_sFileOrDirName;
    if (ueq.m_eEntryType == FOLDERS)
    {
        qDebug() << "FOLDERS";
    }
    else if (ueq.m_eEntryType == FILES)
    {
        qDebug() << "FILES";
    }

    if (ueq.m_eUnequalType == SIZE_UNEQUAL)
    {
        qDebug() << "SIZE_UNEQUAL";
    }
    else if (ueq.m_eUnequalType == ONLY_IN_DIR_1)
    {
        qDebug() << "ONLY_IN_DIR_1";
    }
    else if (ueq.m_eUnequalType == ONLY_IN_DIR_2)
    {
        qDebug() << "ONLY_IN_DIR_2";
    }

    qDebug();
}

bool compareDir(QString strDir1, QString strDir2,
                QList<Unequal> &unequalList)
{
    QDir dir1(strDir1);
    QDir dir2(strDir2);
    if (dir1.exists() == false || dir2.exists() == false)
    {
        return false;
    }

    QStringList dirList1 = dir1.entryList(QDir::AllDirs | QDir::NoDotAndDotDot,
                                          QDir::Name);
    QStringList dirList2 = dir2.entryList(QDir::AllDirs | QDir::NoDotAndDotDot,
                                          QDir::Name);
    QStringList dirList1Only, dirList2Only, dirCommon;
    separateDiffences(dirList1, dirList2,
                      dirList1Only, dirList2Only, dirCommon);
    for (int i = 0; i < dirList1Only.size(); i++)
    {
        Unequal ueq;
        ueq.m_sDir1 = dir1.absolutePath();
        ueq.m_sDir2 = dir2.absolutePath();
        ueq.m_sFileOrDirName = dirList1Only[i];
        ueq.m_eEntryType = FOLDERS;
        ueq.m_eUnequalType = ONLY_IN_DIR_1;
        QString dirName = ueq.m_sDir1 + QDir::separator() + ueq.m_sFileOrDirName;
        ueq.m_iSize1 = calculateDirSize(dirName);
        ueq.m_iSize2 = 0;

        unequalList.append(ueq);
    }
    for (int i = 0; i < dirList2Only.size(); i++)
    {
        Unequal ueq;
        ueq.m_sDir1 = dir1.absolutePath();
        ueq.m_sDir2 = dir2.absolutePath();
        ueq.m_sFileOrDirName = dirList2Only[i];
        ueq.m_eEntryType = FOLDERS;
        ueq.m_eUnequalType = ONLY_IN_DIR_2;
        QString dirName = ueq.m_sDir2 + QDir::separator() + ueq.m_sFileOrDirName;
        ueq.m_iSize1 = 0;
        ueq.m_iSize2 = calculateDirSize(dirName);


        unequalList.append(ueq);
    }
    for (int i = 0; i < dirCommon.size(); i++)
    {
        qint64 dirSize1 = calculateDirSize(strDir1 + QDir::separator() + dirCommon[i]);
        qint64 dirSize2 = calculateDirSize(strDir2 + QDir::separator() + dirCommon[i]);
        if (dirSize1 != dirSize2)
        {
            Unequal ueq;
            ueq.m_sDir1 = dir1.absolutePath();
            ueq.m_sDir2 = dir2.absolutePath();
            ueq.m_sFileOrDirName = dirCommon[i];
            ueq.m_eEntryType = FOLDERS;
            ueq.m_eUnequalType = SIZE_UNEQUAL;
            ueq.m_iSize1 = dirSize1;
            ueq.m_iSize2 = dirSize2;

            unequalList.append(ueq);
        }
        else
        {
            QList<Unequal> subUnequalList;
            compareDir(strDir1 + QDir::separator() + dirCommon[i],
                       strDir2 + QDir::separator() + dirCommon[i],
                       subUnequalList);
            if (subUnequalList.size())
            {
                Unequal ueq;
                ueq.m_sDir1 = dir1.absolutePath();
                ueq.m_sDir2 = dir2.absolutePath();
                ueq.m_sFileOrDirName = dirCommon[i];
                ueq.m_eEntryType = FOLDERS;
                ueq.m_eUnequalType = DETAIL_DIFFERENT;
                ueq.m_iSize1 = dirSize1;
                ueq.m_iSize2 = dirSize2;

                unequalList.append(ueq);
            }
        }
    }

    QStringList fileList1 = dir1.entryList(QDir::Files | QDir::NoDotAndDotDot,
                                           QDir::Name);
    QStringList fileList2 = dir2.entryList(QDir::Files | QDir::NoDotAndDotDot,
                                           QDir::Name);
    QList<Unequal> unequalListOfFile;
    bool bFile = compareFiles(dir1, dir2, fileList1, fileList2, unequalListOfFile);
    if (bFile)
    {
        for (int i = 0; i < unequalListOfFile.size(); i++)
        {
            unequalList.append(unequalListOfFile[i]);
        }
    }

    return true;
}

bool compareFiles(QDir dir1, QDir dir2,
                  QStringList fileList1, QStringList fileList2,
                  QList<Unequal> &unequalList)
{
    unequalList.clear();

    if (dir1.exists() == false || dir2.exists() == false)
    {
        return false;
    }

    QStringList fileList1Only;
    QStringList fileList2Only;
    QStringList fileList12Common;
    separateDiffences(fileList1, fileList2,
                      fileList1Only, fileList2Only, fileList12Common);

    QStringList commonButUnequal;
    for (int i = 0; i < fileList12Common.size(); i++)
    {
        QString name = fileList12Common[i];
//        QFile file1(dir1.absoluteFilePath(name));
//        QFile file2(dir2.absoluteFilePath(name));
//        bool b1 = file1.exists();
//        bool b2 = file2.exists();
        QFileInfo fi1(dir1.absoluteFilePath(name));
        QFileInfo fi2(dir2.absoluteFilePath(name));
        bool b1 = fi1.exists();
        bool b2 = fi2.exists();
        if (b1 && b2)
        {
            if (fi1.size() != fi2.size())
            {
                if (commonButUnequal.contains(name) == false)
                {
                    commonButUnequal.append(name);
                }
            }
        }
        else
        {
            /// 我操脑子抽了吧
        }
    }

    fileList1Only.sort();
    fileList2Only.sort();
    commonButUnequal.sort();

    for (int i = 0; i < fileList1Only.size(); i++)
    {
        Unequal ueq;
        ueq.m_sDir1 = dir1.absolutePath();
        ueq.m_sDir2 = dir2.absolutePath();
        ueq.m_sFileOrDirName = fileList1Only[i];
        ueq.m_eEntryType = FILES;
        ueq.m_eUnequalType = ONLY_IN_DIR_1;
        QFileInfo f(ueq.m_sDir1 + QDir::separator() + ueq.m_sFileOrDirName);
        ueq.m_iSize1 = f.size();
        ueq.m_iSize2 = 0;

        unequalList.append(ueq);
    }
    for (int i = 0; i < fileList2Only.size(); i++)
    {
        Unequal ueq;
        ueq.m_sDir1 = dir1.absolutePath();
        ueq.m_sDir2 = dir2.absolutePath();
        ueq.m_sFileOrDirName = fileList2Only[i];
        ueq.m_eEntryType = FILES;
        ueq.m_eUnequalType = ONLY_IN_DIR_2;
        QFileInfo f(ueq.m_sDir2 + QDir::separator() + ueq.m_sFileOrDirName);
        ueq.m_iSize1 = 0;
        ueq.m_iSize2 = f.size();

        unequalList.append(ueq);
    }
    for (int i = 0; i < commonButUnequal.size(); i++)
    {
        Unequal ueq;
        ueq.m_sDir1 = dir1.absolutePath();
        ueq.m_sDir2 = dir2.absolutePath();
        ueq.m_sFileOrDirName = commonButUnequal[i];
        ueq.m_eEntryType = FILES;
        ueq.m_eUnequalType = SIZE_UNEQUAL;
        QFileInfo f1(ueq.m_sDir1 + QDir::separator() + ueq.m_sFileOrDirName);
        QFileInfo f2(ueq.m_sDir2 + QDir::separator() + ueq.m_sFileOrDirName);
        ueq.m_iSize1 = f1.size();
        ueq.m_iSize2 = f2.size();

        unequalList.append(ueq);
    }

    return true;
}

qint64 calculateDirSize(QString dirName)
{
    QDir dir(dirName);
    qint64 size = 0;

    //entryInfoList(Qdir::Files)函数返回文件信息，根据这些信息计算文件的大小
    foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        size += fileInfo.size();
    }

    // 判断是否有子目录,如果有，则递归计算
    //entryList(QDir::Dirs | QDir::NoDotAndDotDot 返回所有子目录并过滤掉
    // '.' '..'目录
    //使用QDir::separator() 函数来返回特定平台的目录分割符
    foreach (QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        size += calculateDirSize(dirName + QDir::separator() + subDir);
    }

    return size;
}

void separateDiffences(QStringList stl1, QStringList stl2,
                       QStringList &stl1Only, QStringList &stl2Only, QStringList &stlCommon)
{
    stl1Only.clear();
    stl2Only.clear();
    stlCommon.clear();

    for (int i = 0; i < stl1.size(); i++)
    {
        QString str = stl1[i];
        if (stl2.contains(str) == false)
        {
            if (stl1Only.contains(str) == false)
            {
                stl1Only.append(str);
            }
        }
        else
        {
            if (stlCommon.contains(str) == false)
            {
                stlCommon.append(str);
            }
        }
    }

    for (int i = 0; i < stl2.size(); i++)
    {
        QString str = stl2[i];
        if (stl1.contains(str) == false)
        {
            if (stl2Only.contains(str) == false)
            {
                stl2Only.append(str);
            }
        }
        else
        {
            if (stlCommon.contains(str) == false)
            {
                stlCommon.append(str);
            }
        }
    }
}

QString getScientificNotationOfQInt64(qint64 num)
{
    QString str = QString::number(num);
    int size = str.size();
    int segmentSize = size / 3;
    int firstSegmentSize = size - (segmentSize * 3);
    if (firstSegmentSize == 0)
    {
        firstSegmentSize = 3;
    }
    QStringList stl;
    stl.append(str.left(firstSegmentSize));
    str.remove(0, firstSegmentSize);

    for (int i = 0; ; i++)
    {
        if (str.size() < 3)
        {
            if (str.size() == 0)
            {
                break;
            }
            else
            {
                printf("If you can see this, it's means that the program is not stable, please contact developer.\nThe error code is 0\n");
                fflush(0);
                /// 我操出问题了！
            }
        }

        stl.append(str.left(3));
        str.remove(0, 3);
    }

    QString final = stl.join(",");
    return final;
}
