#include "projectloader.h"

#include <QFileInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QDebug>

ProjectLoader::ProjectLoader(ProjectInterface *project) : QObject()
{
    m_project = project;
}

bool ProjectLoader::load(QString path)
{
    QByteArray byteArray;

    QFile file(path);

    if(!file.open(QFile::ReadOnly))
        return false;

    byteArray = file.readAll();
    file.close();

    QFileInfo fileInfo(path);
    m_name = fileInfo.fileName();
    m_workDir = fileInfo.absolutePath();

    emit loading();

    if(!byteArray.isEmpty())
    {
        QJsonDocument jDocument = QJsonDocument::fromJson(byteArray);
        QJsonObject jObject = jDocument.object();

        QStringList fileTypeList = m_project->fileTypeList();
        foreach(const QString &type, fileTypeList)
        {
            if(type.isEmpty())
                continue;

            QJsonArray jArray = jObject[type].toArray();

            foreach(const QJsonValue &jValue, jArray)
            {
                QString path = jValue.toString();
                fileAdd(path, type);
            }
        }
    }

    emit loaded();

    return true;
}

bool ProjectLoader::save()
{
    QString path = m_workDir + "/" + m_name;
    return save(path);
}

bool ProjectLoader::save(QString path)
{
    QJsonDocument jDocument;
    QJsonObject jObject;
    QMap<QString, QJsonArray> jArrayMap;

    QList<int> indexList = m_fileMap.keys();
    foreach(const int &index, indexList)
    {
        QString path = m_fileMap[index].path;
        QString type = m_fileMap[index].type;

        if(path.isEmpty())
            continue;

        if(type.isEmpty())
            continue;

        jArrayMap[type].append(path);
    }

    QStringList fileTypeList = m_project->fileTypeList();
    foreach(const QString &type, fileTypeList)
    {
        if(type.isEmpty())
            continue;

        jObject[type] = jArrayMap[type];
    }

    jDocument.setObject(jObject);

    QFile file(path);

    if(!file.open(QFile::WriteOnly))
        return false;

    file.write(jDocument.toJson());
    file.close();

    return true;
}

int ProjectLoader::fileAdd(QString path, QString type)
{
    QString dir = m_project->fileTypeDir(type);
    QFileInfo fileInfo(path);

    QString absolutePath = fileInfo.absolutePath();
    QString _absolutePath = m_workDir;

    if(!dir.isEmpty())
        _absolutePath += "/" + dir;

    if(absolutePath == _absolutePath)
        path = fileInfo.fileName();

    int index = -1;

    for(int i = 0; i <= m_fileMap.count(); i++)
    {
        if(m_fileMap.contains(i))
            continue;

        index = i;
        break;
    }

    if(index >= 0)
    {
        FileInfo fi;
        fi.path = path;
        fi.type = type;

        m_fileMap[index] = fi;

        save();

        emit fileAdded(index);
    }

    return index;
}

bool ProjectLoader::fileRemove(QString path)
{
    int index = fileIndex(path);
    fileRemove(index);

    return true;
}

bool ProjectLoader::fileRemove(int index)
{
    emit fileRemovedPre(index);

    m_fileMap.remove(index);
    save();

    emit fileRemoved(index);

    return true;
}

QString ProjectLoader::filePath(int index)
{
    return m_fileMap[index].path;
}

QString ProjectLoader::fileType(int index)
{
    return m_fileMap[index].type;
}

int ProjectLoader::fileIndex(QString path)
{
    QList<int> indexList = m_fileMap.keys();
    foreach(const int &index, indexList)
    {
        QString _path = m_fileMap[index].path;

        if(_path == path)
            return index;
    }

    return -1;
}

QStringList ProjectLoader::fileList(QString type)
{
    QStringList value;

    QList<int> indexList = m_fileMap.keys();
    foreach(const int &index, indexList)
    {
        QString _type = m_fileMap[index].type;

        if(!type.isEmpty() && _type != type)
            continue;

        QString path = m_fileMap[index].path;
        value.append(path);
    }

    return value;
}

QString ProjectLoader::name()                   {   return m_name;      }
QString ProjectLoader::workDir()                {   return m_workDir;   }

ProjectInterface *::ProjectLoader::project()    {   return m_project;   }
