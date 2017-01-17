#ifndef BASEPROJECT_H
#define BASEPROJECT_H

#include <QObject>
#include <QtPlugin>
#include <QMap>

#include "../FanxyStudio/projectinterface.h"
#include "../FanxyStudio/projectloaderinterface.h"

#if defined BASEPROJECT
 #define BASEPROJECT_COMMON_DLLSPEC Q_DECL_EXPORT
#else
 #define BASEPROJECT_COMMON_DLLSPEC Q_DECL_IMPORT
#endif

class BASEPROJECT_COMMON_DLLSPEC BaseProject :/* public QObject, */public ProjectInterface
{
    Q_INTERFACES(ProjectInterface)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "FanxyStudio.BaseProjectInterface" FILE "BaseProject.json")

protected : struct FileInfo
{
public:
    QString path;
    QString type;
};

public:
    explicit BaseProject();
    virtual ~BaseProject() {}

    virtual ProjectInterface    *create();

    QString                     fileTypeByFileName(QString name);
    QString                     fileTypeExtension(QString type);
    QString                     fileTypeDir(QString type);
    QString                     fileTypeTemplate(QString type);
    QString                     fileTypeDefault();
    QStringList                 fileTypeList();

    QString                     extension();
    QString                     syntaxTemplate();
    QList<CompleterInfo>        completerInfoList();

    virtual void                build(BuildKey key = BUILD_RELEASE);

    QString                     projectName();
    QString                     projectWorkDir();

    QString                     projectFilePath(int index);
    QString                     projectFileAbsolutePath(int index);
    QString                     projectFileType(int index);
    int                         projectFileIndex(QString path);
    QStringList                 projectFileList(QString type, bool absolutePath = false);

    virtual QWidget             *setupWidget();
    //virtual QWidget             *buildOutputWidget();

    bool                        projectFileExits(QString path, QString type);

    QStringList                 projectHighlightList();

    virtual void                compile(QString path);

    virtual bool                isCompileSupported() {return false;}

    QList<IssueInfo>            projectBuildIssues();
    QString                     projectBuildOutput();

    int                         projectMainFile();
    void                        projectSetMainFile(int index);

public slots:
    bool                        projectLoad(QString path);
    bool                        projectSave();
    bool                        projectSave(QString path);

    int                         projectFileAdd(QString path, QString type);
    bool                        projectFileRemove(int index);
    bool                        projectFileRemove(QString path);

    virtual void                projectFileInfoUpdateRequest(int index);

signals:
    void                        projectLoading();
    void                        projectLoaded();

    void                        projectBuildStarted();
    void                        projectBuildStatus(int current, int total);
    void                        projectBuildFinished();

    void                        projectFileAdded(int index);
    void                        projectFileRemoved(int index);
    void                        projectFileRemovedPre(int index);

protected:
    QString                     m_extension;
    QString                     m_typeDefault;
    QMap<QString, FileTypeInfo> m_fileTypeInfoMap;
    QString                     m_syntaxTemplate;
    QList<CompleterInfo>        m_completerInfoList;
    QStringList                 m_projectHighlightList;

    QString                     m_name;
    QString                     m_workDir;
    QMap<int, FileInfo>         m_fileMap;

    QList<ProjectInterface*>    m_createList;

    QList<IssueInfo>            m_buildIssues;
    QString                     m_buildOutput;

    int                         m_projectMainFile;

    virtual bool                loadConfigMain();
    bool                        loadConfigCompleter();

};

#endif // BASEPROJECT_H
