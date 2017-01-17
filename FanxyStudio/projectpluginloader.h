#ifndef PROJECTPLUGINLOADER_H
#define PROJECTPLUGINLOADER_H

#include <QObject>
#include <QMap>

#include "projectinterface.h"

#if defined(Q_OS_LINUX)
    #define EXT_LIBRARY  "so"
#elif defined(Q_OS_WIN)
    #define EXT_LIBRARY  "dll"
#endif

class ProjectPluginLoader : public QObject
{
    Q_OBJECT

public:
    static ProjectPluginLoader *instance();
    explicit ProjectPluginLoader();

    ProjectInterface                    *projectLoad(QString projectName);
    ProjectInterface                    *projectInstance(QString projectName);
    QStringList                         projectList();

protected:
    void                                loadProjectList();
    void                                loadProjectInfo();

    static ProjectPluginLoader          *m_instance;

    QStringList                         m_projectList;
    QMap<QString, ProjectInterface*>    m_projectInstanceMap;
};

#endif // PROJECTPLUGINLOADER_H
