#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <QObject>
#include <QMap>
#include <QPluginLoader>

#include "projectinterface.h"

class ProjectLoader : public QObject
{
    Q_OBJECT

protected : struct FileInfo
{
public:
    QString path;
    QString type;
};

public:
    explicit ProjectLoader(ProjectInterface *project);

    //Project
    QString             name();
    QString             workDir();
    ProjectInterface    *project();

    //File
    QString             filePath(int index);
    QString             fileType(int index);
    int                 fileIndex(QString path);
    QStringList         fileList(QString type);

public slots:
    //Project
    bool                load(QString path);
    bool                save();
    bool                save(QString path);

    //File
    int                 fileAdd(QString path, QString type);
    bool                fileRemove(int index);
    bool                fileRemove(QString path);

signals:
    //Project
    void                loading();
    void                loaded();

    //File
    void                fileAdded(int index);
    void                fileRemoved(int index);
    void                fileRemovedPre(int index);

protected:
    QString             m_name;
    QString             m_workDir;
    ProjectInterface    *m_project;
    QMap<int, FileInfo> m_fileMap;
};

#endif // PROJECTLOADER_H
