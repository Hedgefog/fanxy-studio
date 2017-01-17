#include "projectpluginloader.h"

#include <QPluginLoader>
#include <QDir>

#include "logger.h"

ProjectPluginLoader *ProjectPluginLoader::m_instance = NULL;

ProjectPluginLoader *ProjectPluginLoader::instance()
{
    if (!m_instance)
        m_instance = new ProjectPluginLoader;

    return m_instance;
}

ProjectPluginLoader::ProjectPluginLoader()
{
    loadProjectList();
    loadProjectInfo();

}

void ProjectPluginLoader::loadProjectList()
{
    QDir dir(DIR_PLUGINS_PROJECTS);
    dir.setFilter(QDir::Files);

    QStringList entryList = dir.entryList();

    foreach(const QString &name, entryList)
    {
        int size = name.size();
        int index = name.lastIndexOf('.') + 1;
        QString ext = name.right(size - index);

        if(ext == EXT_LIBRARY)
        {
            QString projectName = name.left(index - 1);
#if defined(Q_OS_LINUX)
            projectName.remove(0, QString("lib").length());
#endif
            m_projectList.append(projectName);
        }
    }
}

void ProjectPluginLoader::loadProjectInfo()
{
    foreach(QString projectName, m_projectList)
    {
        if(projectName.isEmpty())
            return;

    QString fileName = projectName;

    #if defined(Q_OS_LINUX)
        fileName = "lib" + projectName;
    #endif

        QString workDir = QDir::currentPath();
        QString libDir = workDir + "/" + QString(DIR_PLUGINS_PROJECTS);
        QString libPath = libDir + "/" + fileName + "." + QString(EXT_LIBRARY);

        QDir::setCurrent(libDir);
        QPluginLoader pluginLoader(libPath);
        pluginLoader.load();
        QDir::setCurrent(workDir);

        if(!pluginLoader.isLoaded())
        {
            Logger::write(pluginLoader.errorString());
            return;
        }

        QObject *plugin = pluginLoader.instance();

        ProjectInterface *instance = qobject_cast<ProjectInterface*>(plugin);

        if(instance)
            m_projectInstanceMap[projectName] = instance;
    }
}

QStringList ProjectPluginLoader::projectList()
{
    return m_projectList;
}

ProjectInterface *ProjectPluginLoader::projectInstance(QString projectName)
{
    return m_projectInstanceMap[projectName];
}

ProjectInterface *ProjectPluginLoader::projectLoad(QString projectName)
{
    ProjectInterface *instance = projectInstance(projectName);

    if(!instance)
        return NULL;

    ProjectInterface *project = instance->create();

    if(project)
        Logger::write("Project plugin loaded.");
    else
        Logger::write("Cannot load project plugin.");

    return project;
}
