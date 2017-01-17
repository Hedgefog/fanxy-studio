#include "skinloader.h"

#include <QFile>
#include <QDir>
#include <QColor>
#include <QFont>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "logger.h"

#include <QDebug>

SkinLoader *SkinLoader::m_instance = NULL;

SkinLoader *SkinLoader::instance()
{
    if (!m_instance)
        m_instance = new SkinLoader;

    return m_instance;
}

SkinLoader::SkinLoader(QObject *parent) : QObject(parent)
{
    loadSkinList();
}

void SkinLoader::load(QString name)
{
    QString path = QString(DIR_SKINS) + "/" + name;

    loadConfig(path + "/" + FILE_SKIN_CONFIG);
    loadStyleSheet(path + "/" + FILE_SKIN_STYLESHEET);
    loadSyntaxFormat(path + "/" + FILE_SKIN_STYLE_SYNTAX);
    loadEditorFormat(path + "/" + FILE_SKIN_STYLE_EDITOR);

    emit loaded();
}

void SkinLoader::loadConfig(QString path)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly)) return;
    m_iconsPack = QJsonDocument::fromJson(file.readAll()).object().value("ICONSPACK").toString();
    file.close();
}

void SkinLoader::loadStyleSheet(QString path)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly)) return;
    m_styleSheet = file.readAll();
    file.close();
}

void SkinLoader::loadSyntaxFormat(QString path)
{
    m_syntaxFormatMap = getFormatMap(path);
}

void SkinLoader::loadEditorFormat(QString path)
{
    m_editorFormatMap = getFormatMap(path);
}

void SkinLoader::loadSkinList()
{
    QDir dir(DIR_SKINS);
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    QStringList entryList = dir.entryList();

    foreach(const QString &name, entryList)
    {
        QString path = QString(DIR_SKINS) + "/" + name + "/" + FILE_SKIN_STYLESHEET;

        if(QFile(path).exists())
            m_skinList.append(name);
    }
}

QMap<QString, QTextCharFormat> SkinLoader::getFormatMap(QString path)
{
    QMap<QString, QTextCharFormat> formatMap;

    QByteArray byteArray;

    QFile file(path);

    if(!file.open(QFile::ReadOnly))
        return QMap<QString, QTextCharFormat>();

    byteArray = file.readAll();
    file.close();

    QJsonDocument jDocument = QJsonDocument::fromJson(byteArray);
    QJsonObject jObject = jDocument.object();

    foreach(const QString &style, jObject.keys())
    {
        QJsonObject jKeyObject  = jObject[style].toObject();
        QTextCharFormat format = formatFromJsonObject(jKeyObject);
        formatMap[style]  = format;
    }

    return formatMap;
}

QTextCharFormat SkinLoader::formatFromJsonObject(const QJsonObject &jObject)
{
    QString backgroundColor     = jObject["background-color"].toString();
    QString foregroundColor     = jObject["color"].toString();
    bool italic                 = jObject["italic"].toBool();
    bool bold                   = jObject["bold"].toBool();
    QString fontFamily          = jObject["font-family"].toString();
    int fontSize                = jObject["font-size"].toInt();

    QTextCharFormat format;

    if(isValidColorName(backgroundColor))
    {
        QColor color;
        color.setNamedColor(backgroundColor);

        if(color.isValid())
            format.setBackground(color);
    }

    if(isValidColorName(foregroundColor))
    {
        QColor color;
        color.setNamedColor(foregroundColor);

        if(color.isValid())
            format.setForeground(color);
    }

    format.setFontItalic(italic);

    if(bold)
        format.setFontWeight(QFont::Bold);
    else
        format.setFontWeight(QFont::Normal);

    QFont font;
    font.setFamily(fontFamily);

    if(fontSize > 0)
        font.setPointSize(fontSize);

    format.setFont(font);

    return format;
}

bool SkinLoader::isValidColorName(QString colorName)
{
    if(colorName.isEmpty())
        return false;

    if(colorName.length() != 7)
        return false;

    if(colorName[0] != '#')
        return false;

    return true;
}


QStringList SkinLoader::skinList()                          {   return m_skinList;              }
QString SkinLoader::iconsPack()                             {   return m_iconsPack;             }
QString SkinLoader::styleSheet()                            {   return m_styleSheet;            }
QTextCharFormat SkinLoader::syntaxFormat(const QString key) {   return m_syntaxFormatMap[key];  }
QTextCharFormat SkinLoader::editorFormat(const QString key) {   return m_editorFormatMap[key];  }
