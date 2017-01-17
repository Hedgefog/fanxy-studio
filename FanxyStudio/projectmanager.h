#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QTreeWidget>
#include <QProgressBar>
#include <QMap>
#include <QAction>
#include <QMenu>
#include <QToolBar>

#include <QListWidget>

#include "fileedit.h"
#include "projectinterface.h"
#include "inputdialog.h"
#include "createprojectdialog.h"

#include "skinloader.h"

class ProjectManager : public QObject
{
    Q_OBJECT

public : enum
{
    ITEMTYPE_UNDEFINED,
    ITEMTYPE_PROJECT,
    ITEMTYPE_CATEGORY,
    ITEMTYPE_FILE
};

protected : struct ProjectInfo
{
  QTreeWidgetItem *item;
  QMap<QString, QTreeWidgetItem*> categoryMap;
  QMap<int, QTreeWidgetItem*> fileMap;
};

public:
    explicit ProjectManager(QTreeWidget *treeWidget,
                            FileEdit *fileEdit,
                            QObject *parent = 0);
    ~ProjectManager();

    QTreeWidgetItem                         *currentProjectItem();
    ProjectInterface                        *currentProject();

    QTreeWidgetItem                         *currentCategoryItem();
    QString                                 currentCategory();

    QTreeWidgetItem                         *currentFileItem();
    int                                     currentFile();

    void                                    setProgressBar(QProgressBar *progressBar);
    void                                    setBuildOutputWidget(QPlainTextEdit *widget);
    void                                    setBuildIssuesWidget(QListWidget *widget);
    void                                    setMenu(QMenu *menu);
    void                                    setToolBar(QToolBar *toolBar);

public slots:
    bool                                    projectNew(ProjectInterface *project);
    QString                                 projectOpen(ProjectInterface *project);
    void                                    projectOpen(ProjectInterface *project, QString path);
    bool                                    projectClose(ProjectInterface *project);
    bool                                    projectClose();
    bool                                    projectCloseAll();

    void                                    projectFileOpen(ProjectInterface *project, int index);
    bool                                    projectFileAdd();
    bool                                    projectFileNew();
    bool                                    projectFileRemove();
    bool                                    projectFileRename();
    void                                    projectFileSetMain();
    void                                    projectFileSaveAll(ProjectInterface *project);

    void                                    projectBuildRelease();
    void                                    projectBuildDebug();
    void                                    projectRun();
    void                                    projectRunDebug();

protected slots:
    void                                    on_project_fileAdded(int index);
    void                                    on_project_fileRemovedPre(int index);
    void                                    on_project_loading();
    void                                    on_project_loaded();

    void                                    on_project_buildStarted();
    void                                    on_project_buildFinished();
    void                                    on_project_buildStatus(int current, int total);

    void                                    on_treeWidgetItem_doubleClicked(QTreeWidgetItem *item, int);
    void                                    on_createFileDialog_accepted(QString fileName);
    void                                    on_renameFileDialog_accepted(QString fileName);
    void                                    on_createProjectDialog_accepted(ProjectInterface *project, QString path);

    void                                    on_fileEdit_fileSaved(QString path);

    void                                    on_skinLoader_loaded();

    void                                    on_treeWidgetItem_customContextMenuRequested(QPoint);

    void                                    on_issueWidget_itemClicked(QListWidgetItem *item);


protected:
    QProgressBar                            *m_progressBar;
    QMenu                                   *m_menu;
    QTreeWidget                             *m_treeWidget;
    FileEdit                                *m_fileEdit;
    InputDialog                             *m_createFileDialog;
    InputDialog                             *m_renameFileDialog;
    CreateProjectDialog                     *m_createProjectDialog;

    QMap<ProjectInterface*, ProjectInfo>    m_projectMap;
    QMap<QTreeWidgetItem*, int>             m_itemTypeMap;

    SkinLoader                              *m_skinLoader;

    QAction                                 *m_actProjectClose;
    QAction                                 *m_actProjectFileNew;
    QAction                                 *m_actProjectFileAdd;
    QAction                                 *m_actProjectFileRemove;
    QAction                                 *m_actProjectFileRename;
    QAction                                 *m_actProjectFileSetMain;
    QAction                                 *m_actProjectBuild;
    QAction                                 *m_actProjectBuildDebug;
    QAction                                 *m_actProjectRun;
    QAction                                 *m_actProjectRunDebug;
    QToolBar                                *m_toolBar;

    QMap<QListWidgetItem*, ProjectInterface::IssueInfo> m_issueInfoItemMap;
    QPlainTextEdit                          *m_buildOutputWidget;
    QListWidget                             *m_buildIssuesWidget;

    bool                                    isBuilding;
};

#endif // PROJECTMANAGER_H
