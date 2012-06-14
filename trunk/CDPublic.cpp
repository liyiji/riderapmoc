
#include "CDPublic.h"

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
        ueq.m_eEntryType = DIRECTORIES;
        ueq.m_eUnequalType = NOT_IN_DIR_2;

        unequalList.append(ueq);
    }
    for (int i = 0; i < dirList2Only.size(); i++)
    {
        Unequal ueq;
        ueq.m_sDir1 = dir1.absolutePath();
        ueq.m_sDir2 = dir2.absolutePath();
        ueq.m_sFileOrDirName = dirList2Only[i];
        ueq.m_eEntryType = DIRECTORIES;
        ueq.m_eUnequalType = NOT_IN_DIR_1;

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
            ueq.m_eEntryType = DIRECTORIES;
            ueq.m_eUnequalType = SIZE_UNEQUAL;

            unequalList.append(ueq);
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
        QFile file1(dir1.absoluteFilePath(name));
        QFile file2(dir2.absoluteFilePath(name));
        bool b1 = file1.exists();
        bool b2 = file2.exists();
        if (b1 && b2)
        {
            if (file1.size() != file2.size())
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
        ueq.m_eUnequalType = NOT_IN_DIR_2;

        unequalList.append(ueq);
    }
    for (int i = 0; i < fileList2Only.size(); i++)
    {
        Unequal ueq;
        ueq.m_sDir1 = dir1.absolutePath();
        ueq.m_sDir2 = dir2.absolutePath();
        ueq.m_sFileOrDirName = fileList2Only[i];
        ueq.m_eEntryType = FILES;
        ueq.m_eUnequalType = NOT_IN_DIR_1;

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

        unequalList.append(ueq);
    }

    return true;
}

void debugUnequal(Unequal ueq)
{
    qDebug() << ueq.m_sDir1 << ueq.m_sDir2 << ueq.m_sFileOrDirName;
    if (ueq.m_eEntryType == 0)
    {
        qDebug() << "Directories";
    }
    else if (ueq.m_eEntryType == 1)
    {
        qDebug() << "Files";
    }

    if (ueq.m_eUnequalType == 0)
    {
        qDebug() << "Size_Unequal";
    }
    else if (ueq.m_eUnequalType == 1)
    {
        qDebug() << "Not_In_Dir_1";
    }
    else if (ueq.m_eUnequalType == 2)
    {
        qDebug() << "Not_In_Dir_2";
    }

    qDebug() << endl;
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
