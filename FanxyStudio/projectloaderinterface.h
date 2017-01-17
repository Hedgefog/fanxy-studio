#ifndef PROJECTLOADERINTERFACE_H
#define PROJECTLOADERINTERFACE_H

#include <QString>

#define ProjectLoaderInterface_iid "FanxyStudio.ProjectLoaderInterface"

class ProjectLoaderInterface
{
private:
    ProjectLoaderInterface();
public:
    ProjectLoaderInterface(QStringList fileTypeList) {Q_UNUSED(fileTypeList)}
    virtual ~ProjectLoaderInterface() {}

    //Project
    virtual QString         name() = 0;
    virtual QString         workDir() = 0;

    //File
    virtual QString         filePath(int index)  = 0;
    virtual QString         fileType(int index)  = 0;
    virtual int             fileIndex(QString path) = 0;
    virtual QStringList     fileList(QString type) = 0;
    virtual QList<int>      fileIndexList() = 0;
    virtual QStringList     fileTypeList() = 0;

public slots:
    //Project
    virtual bool            load(QString path) = 0;
    virtual bool            save() = 0;
    virtual bool            save(QString path) = 0;

    //File
    virtual int             fileAdd(QString path, QString type) = 0;
    virtual bool            fileRemove(int index) = 0;
    virtual bool            fileRemove(QString path) = 0;

signals:
    //Project
    virtual void            loading() = 0;
    virtual void            loaded() = 0;

    //File
    virtual void            fileAdded(int index) = 0;
    virtual void            fileRemoved(int index) = 0;
    virtual void            fileRemovedPre(int index) = 0;
};

Q_DECLARE_INTERFACE(ProjectLoaderInterface, ProjectLoaderInterface_iid)

#endif // PROJECTLOADERINTERFACE_H
