#ifndef SETTINGLOADER_H
#define SETTINGLOADER_H

#include <QObject>
#include <QFont>

#include "projectpluginloader.h"

#define FILE_CONFIG_SETTINGS "configs/config.json"

class SettingLoader : public QObject
{
    Q_OBJECT
public:
    static SettingLoader *instance();
    explicit SettingLoader(QObject *parent = 0);

    void                    load();

    QFont                   font();
    QString                 skin();
    bool                    autoInsert();
    bool                    autoIndent();
    QString                 defHighlightTemplate();

    int                     windowWidth();
    int                     windowHeight();
    bool                    windowIsMaximized();

protected:
    static SettingLoader    *m_instance;
    QFont                   m_font;
    QString                 m_skin;
    bool                    m_autoInsert;
    bool                    m_autoIndent;
    QString                 m_defHighlightTemplate;
    int                     m_windowWidth;
    int                     m_windowHeight;
    bool                    m_windowIsMaximized;

signals:
   void                     loaded();

};

#endif // SETTINGLOADER_H
