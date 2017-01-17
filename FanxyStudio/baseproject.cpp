#include "baseproject.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QDebug>

BaseProject::BaseProject() : QObject()
{
    QDir dir;

    if(!dir.exists(DIR_CONFIG))
        dir.mkdir(DIR_CONFIG);

    qDebug() << "[BaseProject] HELLO!";
    load();
}

bool BaseProject::load()
{
    bool result = loadConfigMain();

    if(!result)
        return false;

    loadConfigCompleter();

    return true;
}

bool BaseProject::loadConfigMain()
{
    QString path =  "://config.json";

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

    m_name              = jObject["name"].toString();
    m_extension         = jObject["extension"].toString();
    m_typeDefault       = jObject["type-default"].toString();
    m_highlightTemplate = jObject["highlight-template"].toString();

    QJsonObject jObjectFileTypes = jObject["file-types"].toObject();
    foreach(const QString &key, jObjectFileTypes.keys())
    {
        QJsonObject jObjectFileTypeInfo     = jObjectFileTypes[key].toObject();
        m_fileTypeInfoMap[key].extension    = jObjectFileTypeInfo["extension"].toString();
        m_fileTypeInfoMap[key].dir          = jObjectFileTypeInfo["dir"].toString();
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

    QStringList fileTypeList = getFileTypeList();
    foreach(const QString &type, fileTypeList)
    {
        if(m_fileTypeInfoMap[type].extension == ext)
            return type;
    }

    return QString();
}

QStringList BaseProject::getFileTypeList()
{
    QStringList list;

    QMap<QString, FILETYPEINFO>::iterator it = m_fileTypeInfoMap.begin();
    for(it = m_fileTypeInfoMap.begin(); it != m_fileTypeInfoMap.end(); it++)
        list.append(it.key());

    return list;
}

BaseProject::FILETYPEINFO   BaseProject::fileTypeInfo(QString type) {   return m_fileTypeInfoMap[type];   }

QString                                     BaseProject::name()                     {   return m_name;                  }
QString                                     BaseProject::extension()                {   return m_extension;             }
QString                                     BaseProject::typeDefault()              {   return m_typeDefault;           }
QMap<QString, BaseProject::FILETYPEINFO>    BaseProject::fileTypeInfoMap()          {   return m_fileTypeInfoMap;       }
QList<BaseProject::COMPLETERINFO>           BaseProject::completerInfoList()        {   return m_completerInfoList;     }
QString                                     BaseProject::highlightTemplate()        {   return m_highlightTemplate;     }

void    BaseProject::compile()      {}
void    BaseProject::compileDebug() {}
void    BaseProject::run()          {}
void    BaseProject::runDebug()     {}
