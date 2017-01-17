#ifndef PROJECTINTERFACE_H
#define PROJECTINTERFACE_H

#include <QObject>
#include <QMap>
#include <QLayout>
#include <QWidget>
#include <QSize>

#define DIR_CONFIGS                 "configs"
#define DIR_PLUGINS_PROJECTS        "plugins/projects"

#define ProjectInterface_iid        "FanxyStudio.ProjectInterface"

#define SIZE_WIDGET_PLUGIN_SETUP    QSize(320, 256)

class ProjectInterface : public QObject
{
public : enum
{
    ISSUE_NONE = 0,
    ISSUE_WARNING,
    ISSUE_ERROR,
    ISSUE_FATAL_ERROR
};

public : enum BuildKey
{
    BUILD_RELEASE = 0,
    BUILD_DEBUG,
    BUILD_RUN,
    BUILD_RUN_DEBUG
};

public : struct IssueInfo
{
    QString filePath;
    QString errorString;
    int line;
    int type;
};

protected : struct FileTypeInfo
{
    QString extension;
    QString dir;
    QString fileTemplate;
};

public : struct CompleterInfo
{
    QString name;
    QString info;
};
public:
    virtual                         ~ProjectInterface() {}

    virtual ProjectInterface        *create() = 0;

    virtual QString                 extension() = 0;
    virtual QString                 syntaxTemplate() = 0;
    virtual QList<CompleterInfo>    completerInfoList() = 0;

    virtual QString                 fileTypeByFileName(QString name) = 0;
    virtual QString                 fileTypeExtension(QString type) = 0;
    virtual QString                 fileTypeDir(QString type) = 0;
    virtual QString                 fileTypeDefault() = 0;
    virtual QStringList             fileTypeList() = 0;
    virtual QString                 fileTypeTemplate(QString type) = 0;

    virtual void                    build(BuildKey key = BUILD_RELEASE) = 0;

    virtual QString                 projectName() = 0;
    virtual QString                 projectWorkDir() = 0;

    virtual QString                 projectFilePath(int index) = 0;
    virtual QString                 projectFileAbsolutePath(int index) = 0;
    virtual QString                 projectFileType(int index) = 0;
    virtual int                     projectFileIndex(QString path) = 0;
    virtual QStringList             projectFileList(QString type, bool absolutePath = false) = 0;
    virtual bool                    projectFileExits(QString path, QString type) = 0;

    virtual QStringList             projectHighlightList() = 0;

    virtual void                    compile(QString path) = 0;

    virtual bool                    isCompileSupported() = 0;

    virtual QWidget                 *setupWidget() = 0;

    virtual QList<IssueInfo>        projectBuildIssues() = 0;
    virtual QString                 projectBuildOutput() = 0;

    virtual int                     projectMainFile() = 0;
    virtual void                    projectSetMainFile(int index) = 0;

public slots:
    virtual void                    projectFileInfoUpdateRequest(int index) = 0;

    virtual bool                    projectLoad(QString path) = 0;
    virtual bool                    projectSave() = 0;
    virtual bool                    projectSave(QString path) = 0;

    virtual int                     projectFileAdd(QString path, QString type) = 0;
    virtual bool                    projectFileRemove(int index) = 0;
    virtual bool                    projectFileRemove(QString path) = 0;

public:
    virtual void                    projectLoading() = 0;
    virtual void                    projectLoaded() = 0;

    virtual void                    projectBuildStarted() = 0;
    virtual void                    projectBuildStatus(int current, int total) = 0;
    virtual void                    projectBuildFinished() = 0;

    virtual void                    projectFileAdded(int index) = 0;
    virtual void                    projectFileRemoved(int index) = 0;
    virtual void                    projectFileRemovedPre(int index) = 0;
};

Q_DECLARE_INTERFACE(ProjectInterface, ProjectInterface_iid)

#endif // PROJECTINTERFACE_H
