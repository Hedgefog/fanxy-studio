#include "baseproject.h"

Q_PLUGIN_METADATA(IID ProjectInterface_iid)

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QDebug>

#include <QResource>

#include <QBoxLayout>

BaseProject::BaseProject()
{
    loadConfigMain();

    m_projectMainFile = 0;

    //QString className = metaObject()->className();
    //qDebug() << "[" <<  className << "] Loaded!";
}

ProjectInterface *BaseProject::create()
{
    ProjectInterface *created = new BaseProject;
    m_createList.append(created);
    return created;
}

bool BaseProject::loadConfigMain()
{
    QString className = metaObject()->className();
    QString path = ":/" + className + "/config.json";

    QByteArray byteArray;

    QFile file(path);

    if(!file.open(QFile::ReadOnly))
        return false;

    byteArray = file.readAll();
    file.close();

    if(byteArray.isEmpty())
        return false;

    QJsonDocument jDocument = QJsonDocument::fromJson(byteArray);
    QJsonObject jObject = jDocument.object();

    if(jObject.isEmpty())
        return false;

    m_extension         = jObject["extension"].toString();
    m_typeDefault       = jObject["type-default"].toString();
    m_syntaxTemplate    = jObject["syntax-template"].toString();

    m_fileTypeInfoMap.clear();

    QJsonObject jObjectFileTypes = jObject["file-types"].toObject();
    foreach(const QString &key, jObjectFileTypes.keys())
    {
        QJsonObject jObjectFileTypeInfo     = jObjectFileTypes[key].toObject();
        m_fileTypeInfoMap[key].extension    = jObjectFileTypeInfo["extension"].toString();
        m_fileTypeInfoMap[key].dir          = jObjectFileTypeInfo["dir"].toString();
        m_fileTypeInfoMap[key].fileTemplate = jObjectFileTypeInfo["template"].toString();
    }

    return true;
}

bool BaseProject::loadConfigCompleter()
{
    /*QString path = QString(DIR_CONFIG) + "/" + QString(FILE_CONFIG_COMPLETER);

    QByteArray byteArray;

    QFile file(path);

    if(!file.open(QFile::ReadOnly))
        return false;

    byteArray = file.readAll();
    file.close();

    if(byteArray.isEmpty())
        return false;

    QJsonDocument jDocument = QJsonDocument::fromJson(byteArray);
    QJsonObject jObject = jDocument.object();

    if(jObject.isEmpty())
        return false;

    foreach(const QString &key, jObject.keys())
    {
        QJsonObject jObjectData = jObject[key].toObject();

        COMPLETERINFO completerInfo;
        completerInfo.name = key;
        completerInfo.info = jObjectData["info"].toString();

        m_completerInfoList.append(completerInfo);
    }

    return true;*/

    return false;
}

QString BaseProject::fileTypeByFileName(QString name)
{
    if(name.isEmpty())
        return QString();

    int size = name.size();
    int index = name.lastIndexOf('.') + 1;
    QString ext = name.right(size - index);

    QStringList fileTypeList = m_fileTypeInfoMap.keys();
    foreach(const QString &type, fileTypeList)
    {
        if(m_fileTypeInfoMap[type].extension == ext)
            return type;
    }

    return m_typeDefault;
}

QStringList                                 BaseProject::fileTypeList()                     {   return m_fileTypeInfoMap.keys();                }
QString                                     BaseProject::fileTypeExtension(QString type)    {   return m_fileTypeInfoMap[type].extension;       }
QString                                     BaseProject::fileTypeDir(QString type)          {   return m_fileTypeInfoMap[type].dir;             }
QString                                     BaseProject::fileTypeDefault()                  {   return m_typeDefault;                           }
QString                                     BaseProject::fileTypeTemplate(QString type)     {   return m_fileTypeInfoMap[type].fileTemplate;   }

QString                                     BaseProject::extension()                        {   return m_extension;                             }
QList<BaseProject::CompleterInfo>           BaseProject::completerInfoList()                {   return m_completerInfoList;                     }
QString                                     BaseProject::syntaxTemplate()                   {   return m_syntaxTemplate;                        }

void BaseProject::build(BuildKey key)
{
    Q_UNUSED(key);
    qDebug() << "Build is not supported by this project plugin";
}

bool BaseProject::projectLoad(QString path)
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

    emit projectLoading();

    if(!byteArray.isEmpty())
    {
        QJsonDocument jDocument = QJsonDocument::fromJson(byteArray);
        QJsonObject jObject = jDocument.object();

        QStringList fileTypeList = m_fileTypeInfoMap.keys();
        foreach(const QString &type, fileTypeList)
        {
            if(type.isEmpty())
                continue;

            QJsonArray jArray = jObject[type].toArray();

            foreach(const QJsonValue &jValue, jArray)
            {
                QString path = jValue.toString();
                projectFileAdd(path, type);
            }
        }

        m_projectMainFile = jObject["_INDEX_OF_MAIN_FILE"].toInt();
    }

    emit projectLoaded();

    return true;
}

bool BaseProject::projectSave()
{
    QString path = m_workDir + "/" + m_name;
    return projectSave(path);
}

bool BaseProject::projectSave(QString path)
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

    QStringList fileTypeList = m_fileTypeInfoMap.keys();
    foreach(const QString &type, fileTypeList)
    {
        if(type.isEmpty())
            continue;

        jObject[type] = jArrayMap[type];
    }

    jObject["_INDEX_OF_MAIN_FILE"] = m_projectMainFile;
    jDocument.setObject(jObject);

    QFile file(path);

    if(!file.open(QFile::WriteOnly))
        return false;

    file.write(jDocument.toJson());
    file.close();

    return true;
}

int BaseProject::projectFileAdd(QString path, QString type)
{
    QFileInfo fileInfo(path);

    QString absolutePath = fileInfo.absolutePath();
    QString _absolutePath = m_workDir;

    QString dir = fileTypeDir(type);

    if(!dir.isEmpty())
        _absolutePath += "/" + dir;

    if(absolutePath == _absolutePath)
        path = fileInfo.fileName();

    if(projectFileExits(path, type))
        return -1;

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

        projectSave();

        emit projectFileAdded(index);
    }

    return index;
}

bool BaseProject::projectFileRemove(QString path)
{
    int index = projectFileIndex(path);
    projectFileRemove(index);

    return true;
}

bool BaseProject::projectFileRemove(int index)
{
    emit projectFileRemovedPre(index);

    m_fileMap.remove(index);
    projectSave();

    emit projectFileRemoved(index);

    return true;
}

QString BaseProject::projectFilePath(int index)
{
    return m_fileMap[index].path;
}

QString BaseProject::projectFileAbsolutePath(int index)
{
    QString fileName = m_fileMap[index].path;

    QFileInfo fileInfo(fileName);

    QString path;
    QString type = m_fileMap[index].type;
    QString dir = m_fileTypeInfoMap[type].dir;;

    if(!fileInfo.isAbsolute())
    {
        path += m_workDir;

        if(!dir.isEmpty())
            path += "/" + dir;

        path += "/";
    }

    path += fileName;

    return path;
}

QString BaseProject::projectFileType(int index)
{
    return m_fileMap[index].type;
}

int BaseProject::projectFileIndex(QString path)
{
    QList<int> indexList = m_fileMap.keys();
    foreach(const int &index, indexList)
    {
        QString _path = m_fileMap[index].path;
        QString _type = m_fileMap[index].type;

        if(_path == path)
        {
            return index;
        }
        else
        {
            QFileInfo fileInfo(path);
            QFileInfo _fileInfo(_path);

            if(_fileInfo.isAbsolute())
                continue;

            QString fileName = fileInfo.fileName();

            if(_path != fileName)
                continue;

            QString _absolutePath = m_workDir;
            QString dir = fileTypeDir(_type);

            if(!dir.isEmpty())
                _absolutePath += "/" + dir;

            _absolutePath += "/" + _path;

            if(_absolutePath == path)
                return index;
        }
    }

    return -1;
}

QStringList BaseProject::projectFileList(QString type, bool absolutePath)
{
    QStringList value;

    QList<int> indexList = m_fileMap.keys();
    foreach(const int &index, indexList)
    {
        QString _type = m_fileMap[index].type;

        if(!type.isEmpty() && _type != type)
            continue;

        QString path;

        if(absolutePath)
            path = projectFileAbsolutePath(index);
        else
            path = m_fileMap[index].path;

        value.append(path);
    }

    return value;
}

bool BaseProject::projectFileExits(QString path, QString type)
{
    QList<int> indexList = m_fileMap.keys();
    foreach(const int &index, indexList)
    {
        if((m_fileMap[index].path == path) && (m_fileMap[index].type == type))
            return true;
    }

    return false;
}

QString BaseProject::projectName()                  {   return m_name;                  }
QString BaseProject::projectWorkDir()               {   return m_workDir;               }
QStringList BaseProject::projectHighlightList()     {   return m_projectHighlightList;  }

int BaseProject::projectMainFile()                  {   return m_projectMainFile;       }

void BaseProject::projectSetMainFile(int index)
{
    m_projectMainFile = index;
    projectSave();
}

QList<ProjectInterface::IssueInfo>  BaseProject::projectBuildIssues()    {   return m_buildIssues; }
QString                             BaseProject::projectBuildOutput()    {  return m_buildOutput;}

void BaseProject::projectFileInfoUpdateRequest(int index)
{
    Q_UNUSED(index);
}

void BaseProject::compile(QString path)
{
    Q_UNUSED(path);
}

QWidget *BaseProject::setupWidget()
{
    return NULL;
}

//QWidget *BaseProject::buildOutputWidget()
//{
//    return NULL;
//}
