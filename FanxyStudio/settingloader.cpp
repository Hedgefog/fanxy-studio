#include "settingloader.h"

#include <QFile>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "logger.h"

SettingLoader *SettingLoader::m_instance = NULL;

SettingLoader *SettingLoader::instance()
{
    if (!m_instance)
        m_instance = new SettingLoader;

    return m_instance;
}

SettingLoader::SettingLoader(QObject *parent) : QObject(parent)
{
    m_autoInsert        = false;
    m_autoIndent        = false;
    m_windowWidth       = 0;
    m_windowHeight      = 0;
    m_windowIsMaximized = false;

    Logger::write("Setting loader initialized.");
    load();
}

void SettingLoader::load()
{
    QByteArray byteArray;

    QFile file(QDir::currentPath() + "/" + FILE_CONFIG_SETTINGS);

    Logger::write(file.fileName());

    if(!file.exists())
    {
        QFile::copy("://config.json", FILE_CONFIG_SETTINGS);
        QFile(FILE_CONFIG_SETTINGS).setPermissions(QFile::WriteUser);
    }

    if(!file.open(QFile::ReadOnly))
        return;

    byteArray = file.readAll();
    file.close();

    if(byteArray.isEmpty())
        return;

    QJsonDocument jDocument = QJsonDocument::fromJson(byteArray);
    QJsonObject jObject = jDocument.object();

    if(jObject.isEmpty())
        return;

    m_font                  = QFont(jObject["Font"].toString(), jObject["FontSize"].toInt());
    m_skin                  = jObject["Skin"].toString();
    m_autoInsert            = jObject["AutoInsert"].toBool();
    m_autoIndent            = jObject["AutoIndent"].toBool();
    m_defHighlightTemplate  = jObject["DefHighlightTemplate"].toString();
    m_windowWidth           = jObject["WindowWidth"].toInt();
    m_windowHeight          = jObject["WindowHeight"].toInt();
    m_windowIsMaximized     = jObject["WindowIsMaximized"].toBool();

    emit loaded();
}

QFont       SettingLoader::font()                   {   return m_font;                  }
QString     SettingLoader::skin()                   {   return m_skin;                  }
bool        SettingLoader::autoInsert()             {   return m_autoInsert;            }
bool        SettingLoader::autoIndent()             {   return m_autoIndent;            }
QString     SettingLoader::defHighlightTemplate()   {   return m_defHighlightTemplate;  }
int         SettingLoader::windowWidth()            {   return m_windowWidth;           }
int         SettingLoader::windowHeight()           {   return m_windowHeight;          }
bool        SettingLoader::windowIsMaximized()      {   return m_windowIsMaximized;     }


