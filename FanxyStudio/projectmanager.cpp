#include "projectmanager.h"

#include <QFileDialog>
#include <QAbstractItemModel>

#include <QDebug>

#include <QTextDocument>
#include <QTextCursor>

#include "logger.h"

ProjectManager::ProjectManager(QTreeWidget *treeWidget,
                               FileEdit *fileEdit,
                               QObject *parent) : QObject(parent)
{
    m_progressBar = NULL;
    m_buildOutputWidget = NULL;
    m_buildIssuesWidget = NULL;
    m_menu = NULL;
    m_toolBar = NULL;

    isBuilding = false;

    m_skinLoader = SkinLoader::instance();

    m_treeWidget = treeWidget;
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    m_fileEdit = fileEdit;

    m_renameFileDialog = new InputDialog;
    m_renameFileDialog->setTitle("Rename file");
    m_renameFileDialog->setButtonText("Rename");

    m_createFileDialog = new InputDialog;
    m_createFileDialog->setTitle("Create new file");
    m_createFileDialog->setButtonText("Create");

    m_createProjectDialog = new CreateProjectDialog;

    connect(m_treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(on_treeWidgetItem_doubleClicked(QTreeWidgetItem*, int)));
    connect(m_createFileDialog, SIGNAL(accepted(QString)), this, SLOT(on_createFileDialog_accepted(QString)));
    connect(m_renameFileDialog, SIGNAL(accepted(QString)), this, SLOT(on_renameFileDialog_accepted(QString)));
    connect(m_createProjectDialog, SIGNAL(accepted(ProjectInterface*, QString)), this, SLOT(on_createProjectDialog_accepted(ProjectInterface*, QString)));
    connect(m_fileEdit, SIGNAL(fileSaved(QString)), this, SLOT(on_fileEdit_fileSaved(QString)));
    connect(m_skinLoader, SIGNAL(loaded()), this, SLOT(on_skinLoader_loaded()));
    connect(m_treeWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(on_treeWidgetItem_customContextMenuRequested(QPoint)));

    m_actProjectClose = new QAction("Close Project", this);
    m_actProjectFileNew = new QAction("New File", this);
    m_actProjectFileAdd = new QAction("Add File", this);
    m_actProjectFileRemove = new QAction("Remove File", this);
    m_actProjectFileRename = new QAction("Rename File", this);
    m_actProjectFileSetMain = new QAction("Set as main file", this);
    m_actProjectBuild = new QAction("Build", this);
    m_actProjectBuildDebug = new QAction("Build Debug", this);
    m_actProjectRun = new QAction("Run", this);
    m_actProjectRunDebug = new QAction("Run Debug", this);

    //m_actProjectClose->setShortcut(QKeySequence(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_W));
    m_actProjectFileNew->setShortcut(QKeySequence(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_N));
    m_actProjectFileAdd->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_E));
    m_actProjectFileRemove->setShortcut(QKeySequence(Qt::Key_Delete));
    m_actProjectFileRename->setShortcut(QKeySequence(Qt::Key_F2));
    m_actProjectBuild->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
    m_actProjectBuildDebug->setShortcut(QKeySequence(Qt::CTRL + Qt::ShiftModifier + Qt::Key_B));
    m_actProjectRun->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    m_actProjectRunDebug->setShortcut(QKeySequence(Qt::CTRL+ Qt::ShiftModifier + Qt::Key_R));

    connect(m_actProjectClose, SIGNAL(triggered()), this, SLOT(projectClose()));
    connect(m_actProjectFileAdd, SIGNAL(triggered()), this, SLOT(projectFileAdd()));
    connect(m_actProjectFileNew, SIGNAL(triggered()), this, SLOT(projectFileNew()));
    connect(m_actProjectFileRemove, SIGNAL(triggered()), this, SLOT(projectFileRemove()));
    connect(m_actProjectFileRename, SIGNAL(triggered()), this, SLOT(projectFileRename()));
    connect(m_actProjectFileSetMain, SIGNAL(triggered()), this, SLOT(projectFileSetMain()));
    connect(m_actProjectBuild, SIGNAL(triggered()), this, SLOT(projectBuildRelease()));
    connect(m_actProjectBuildDebug, SIGNAL(triggered()), this, SLOT(projectBuildDebug()));
    connect(m_actProjectRun, SIGNAL(triggered()), this, SLOT(projectRun()));
    connect(m_actProjectRunDebug, SIGNAL(triggered()), this, SLOT(projectRunDebug()));

    Logger::write("Project manager initialized.");
}

ProjectManager::~ProjectManager()
{
  delete m_createFileDialog;
}

void ProjectManager::setProgressBar(QProgressBar *progressBar)
{
    m_progressBar = progressBar;
}

void ProjectManager::setBuildOutputWidget(QPlainTextEdit *widget)
{
    m_buildOutputWidget = widget;
}

void ProjectManager::setBuildIssuesWidget(QListWidget *widget)
{
    m_buildIssuesWidget = widget;
    connect(m_buildIssuesWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_issueWidget_itemClicked(QListWidgetItem*)));
}

void ProjectManager::setMenu(QMenu *menu)
{
    m_menu = menu;
    m_menu->clear();
    m_menu->addAction(m_actProjectFileNew);
    m_menu->addAction(m_actProjectFileAdd);
    m_menu->addAction(m_actProjectFileSetMain);
    m_menu->addSeparator();
    m_menu->addAction(m_actProjectFileRemove);
    m_menu->addAction(m_actProjectFileRename);
    m_menu->addSeparator();
    m_menu->addAction(m_actProjectBuild);
    m_menu->addAction(m_actProjectRun);
    m_menu->addSeparator();
    m_menu->addAction(m_actProjectBuildDebug);
    m_menu->addAction(m_actProjectRunDebug);
}

void ProjectManager::setToolBar(QToolBar *toolBar)
{
    m_toolBar = toolBar;
    //m_toolBar->clear();
    m_toolBar->addAction(m_actProjectFileNew);
    m_toolBar->addAction(m_actProjectFileAdd);
    //m_toolBar->addAction(m_actProjectFileSetMain);
    m_toolBar->addAction(m_actProjectFileRemove);
    //m_toolBar->addAction(m_actProjectFileRename);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_actProjectBuild);
    m_toolBar->addAction(m_actProjectRun);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_actProjectBuildDebug);
    m_toolBar->addAction(m_actProjectRunDebug);
}

void ProjectManager::on_skinLoader_loaded()
{
    m_createFileDialog->setStyleSheet(m_skinLoader->styleSheet());
    m_createProjectDialog->setStyleSheet(m_skinLoader->styleSheet());

    m_actProjectClose->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/projectclose.png"));
    m_actProjectRun->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/run.png"));
    m_actProjectRunDebug->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/rundebug.png"));
    m_actProjectBuild->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/build.png"));
    m_actProjectBuildDebug->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/builddebug.png"));
    m_actProjectFileNew->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/filenew.png"));
    m_actProjectFileAdd->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/fileadd.png"));
    m_actProjectFileRemove->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/fileremove.png"));

    foreach(QListWidgetItem *item, m_issueInfoItemMap.keys())
    {
        QColor backgroundColor;
        QColor foregroundColor;

        switch(m_issueInfoItemMap[item].type)
        {
        case ProjectInterface::ISSUE_WARNING:
            backgroundColor = m_skinLoader->editorFormat("ISSUE_WARNING").background().color();
            foregroundColor = m_skinLoader->editorFormat("ISSUE_WARNING").foreground().color();
            break;
        case ProjectInterface::ISSUE_ERROR:
            backgroundColor = m_skinLoader->editorFormat("ISSUE_ERROR").background().color();
            foregroundColor = m_skinLoader->editorFormat("ISSUE_ERROR").foreground().color();
            break;
        case ProjectInterface::ISSUE_FATAL_ERROR:
            backgroundColor = m_skinLoader->editorFormat("ISSUE_FATAL_ERROR").background().color();
            foregroundColor = m_skinLoader->editorFormat("ISSUE_FATAL_ERROR").foreground().color();
            break;
        }

        if(backgroundColor.isValid())
            item->setBackground(backgroundColor);

        if(foregroundColor.isValid())
            item->setForeground(foregroundColor);
    }
}

bool ProjectManager::projectNew(ProjectInterface *project)
{
    m_createProjectDialog->setProject(project);
    m_createProjectDialog->show();

    return true;
}

void ProjectManager::projectOpen(ProjectInterface *project, QString path)
{
    connect(project, SIGNAL(projectLoading()), this, SLOT(on_project_loading()));
    connect(project, SIGNAL(projectLoaded()), this, SLOT(on_project_loaded()));
    connect(project, SIGNAL(projectBuildStarted()), this, SLOT(on_project_buildStarted()));
    connect(project, SIGNAL(projectBuildFinished()), this, SLOT(on_project_buildFinished()));
    connect(project, SIGNAL(projectBuildStatus(int, int)), this, SLOT(on_project_buildStatus(int, int)));

    project->projectLoad(path);
}

QString ProjectManager::projectOpen(ProjectInterface *project)
{
    if(!project)
        return QString();

    QString ext = project->extension();

    //Setup filter
    QString filter = "Project file (*." + ext +  ")";

    //Open file dialog
    QString path = QFileDialog::getOpenFileName
    (
        NULL,
        "Open file...",
        QString(),
        filter.toUtf8()
    );

    if(path.isEmpty())
        return QString();

    projectOpen(project, path);

    return path;
}

bool ProjectManager::projectClose(ProjectInterface *project)
{
  QTreeWidgetItem *itemProject = m_projectMap[project].item;

  for(int i = 0; i < itemProject->childCount(); i++)
  {
    QTreeWidgetItem *itemCategory = itemProject->child(i);

    for(int k = 0; k < itemCategory->childCount(); k++)
    {
      QTreeWidgetItem *itemFile = itemCategory->child(k);

      itemCategory->removeChild(itemFile);
      delete itemFile;
    }

    itemProject->removeChild(itemCategory);
    delete itemCategory;
  }

  int itemProjectIndex = m_treeWidget->indexOfTopLevelItem(itemProject);
  m_treeWidget->takeTopLevelItem(itemProjectIndex);
  delete itemProject;

  m_projectMap[project].categoryMap.clear();
  m_projectMap[project].fileMap.clear();
  m_projectMap[project].item = NULL;

  m_projectMap.remove(project);

  disconnect(project, SIGNAL(projectFileAdded(int)), this, SLOT(on_project_fileAdded(int)));
  disconnect(project, SIGNAL(projectFileRemovedPre(int)), this, SLOT(on_project_fileRemovedPre(int)));

  Logger::write("Project closed.");

  return true;
}

bool ProjectManager::projectClose()
{
    ProjectInterface *project  = currentProject();

    if(!project)
        return false;

    bool result = projectClose(project);

    return result;
}

bool ProjectManager::projectCloseAll()
{
    QList<ProjectInterface*> projectList = m_projectMap.keys();
    foreach(ProjectInterface *project, projectList)
        projectClose(project);

    return true;
}


void ProjectManager::projectBuildRelease()
{
    if(isBuilding)
        return;

    ProjectInterface *project  = currentProject();

    if(!project)
        return;

    projectFileSaveAll(project);
    project->build(ProjectInterface::BUILD_RELEASE);
}

void ProjectManager::projectBuildDebug()
{
    if(isBuilding)
        return;

    ProjectInterface *project  = currentProject();

    if(!project)
        return;

    projectFileSaveAll(project);
    project->build(ProjectInterface::BUILD_DEBUG);
}

void ProjectManager::projectRun()
{
    if(isBuilding)
        return;

    ProjectInterface *project  = currentProject();

    if(!project)
        return;

    projectFileSaveAll(project);
    project->build(ProjectInterface::BUILD_RUN);
}

void ProjectManager::projectRunDebug()
{
    if(isBuilding)
        return;

    ProjectInterface *project  = currentProject();

    if(!project)
        return;

    projectFileSaveAll(project);
    project->build(ProjectInterface::BUILD_RUN_DEBUG);
}

bool ProjectManager::projectFileAdd()
{
    ProjectInterface *project = currentProject();

    if(!project)
        return false;

    //Setup filter
    QString filter = "All files (*.*)";

    //Open file dialog
    QStringList pathList = QFileDialog::getOpenFileNames
    (
        NULL,
        "Open file...",
        QString(),
        filter.toUtf8()
    );

    if(pathList.isEmpty())
        return false;

    foreach(const QString &path, pathList)
    {
        QString type = project->fileTypeByFileName(path);
        int index = project->projectFileAdd(path, type);
        qDebug() << "File with index " << index << "added to project.";
    }

    return (pathList.count() > 0);
}

bool ProjectManager::projectFileNew()
{
    ProjectInterface *project = currentProject();

    if(!project)
        return false;

    QString type = project->fileTypeDefault();;
    m_createFileDialog->setText("file." + project->fileTypeExtension(type));
    m_createFileDialog->show();

    return true;
}

bool ProjectManager::projectFileRemove()
{
    if(!m_treeWidget->isActiveWindow())
        return false;

    ProjectInterface *project = currentProject();

    if(!project)
        return false;

    int index = currentFile();

    if(index < 0)
        return false;

    bool result = project->projectFileRemove(index);

    Logger::write("Removed file from project.");

    return result;
}

bool ProjectManager::projectFileRename()
{
    if(!m_treeWidget->isActiveWindow())
        return false;

    ProjectInterface *project = currentProject();

    if(!project)
        return false;

    int index = currentFile();

    if(index < 0)
        return false;

    m_renameFileDialog->setText(project->projectFilePath(index));
    m_renameFileDialog->show();

    return true;
}

void ProjectManager::projectFileSetMain()
{
    if(!m_treeWidget->isActiveWindow())
        return;

    ProjectInterface *project = currentProject();

    if(!project)
        return;

    int index = currentFile();

    if(index < 0)
        return;

    project->projectSetMainFile(index);
}

void ProjectManager::on_createFileDialog_accepted(QString fileName)
{
    if(fileName.isEmpty())
        return;

    ProjectInterface *project = currentProject();

    if(!project)
        return;

    QString type = project->fileTypeByFileName(fileName);

    QString dir = project->fileTypeDir(type);
    QString path = project->projectWorkDir();

    if(!dir.isEmpty())
    {
        if(!QDir(path).exists(dir))
            QDir(path).mkdir(dir);

        path += "/" + dir;
    }

    path += "/" + fileName;

    QFile file(path);
    bool isOpened = file.open(QFile::WriteOnly);

    if(!isOpened)
        return;

    file.write(project->fileTypeTemplate(type).toUtf8());
    file.close();

    project->projectFileAdd(fileName, type);

    return;
}

void ProjectManager::on_renameFileDialog_accepted(QString fileName)
{
    ProjectInterface *project = currentProject();

    if(!project)
        return;

    int index = currentFile();

    if(index < 0)
        return;

    QString oldFileName = project->projectFilePath(index);

    if(oldFileName.isEmpty())
        return;

    QString type = project->fileTypeByFileName(oldFileName);

    QString dir = project->fileTypeDir(type);
    QString path = project->projectWorkDir();
    QString newPath;

    if(!dir.isEmpty())
    {
        if(!QDir(path).exists(dir))
            QDir(path).mkdir(dir);

        path += "/" + dir;
    }

    newPath = path + "/" + fileName;
    path += "/" + oldFileName;

    QFile file(path);
    file.rename(newPath);
    file.close();

    project->projectFileRemove(index);
    project->projectFileAdd(fileName, type);

    return;
}

void ProjectManager::on_createProjectDialog_accepted(ProjectInterface *project, QString path)
{
    QFile projectFile(path);

    if(!projectFile.open(QFile::WriteOnly))
        return;

    projectFile.close();

//    QString type = project->fileTypeDefault();
//    QString dir = project->fileTypeDir(type);
//    QString filePath = project->projectWorkDir();
//    QString fileName = project->projectName();
//    fileName = fileName.left(fileName.lastIndexOf('.') + 1) + project->fileTypeExtension(type);

//    if(!dir.isEmpty())
//    {
//        if(!QDir(filePath).exists(dir))
//            QDir(filePath).mkdir(dir);

//        filePath += "/" + dir;
//    }

//    filePath += "/" + fileName;

//    QFile file(filePath);

//    if(file.open(QFile::WriteOnly))
//        return;

//    file.write(project->fileTypeTemplate(type).toUtf8());
//    file.close();

//    project->projectFileAdd(filePath, type);

    projectOpen(project, path);
}

void ProjectManager::projectFileOpen(ProjectInterface *project, int index)
{
    if(index < 0)
        return;

    if(!project)
        return;

    QString fileName = project->projectFilePath(index);
    QString workDir = project->projectWorkDir();

    QFileInfo fileInfo(fileName);
    bool isAbsolute = fileInfo.isAbsolute();

    QString path;
    QString type = project->projectFileType(index);
    QString dir = project->fileTypeDir(type);

    if(!isAbsolute)
    {
        path += workDir;

        if(!dir.isEmpty())
            path += "/" + dir;

        path += "/";
    }

    path += fileName;

    bool isOpened = m_fileEdit->fileOpen(path);

    if(isOpened)
    {
        CodeEdit *codeEdit = m_fileEdit->codeEdit();
        QTextDocument *document = codeEdit->document();
        codeEdit->setSyntaxTemplate(document, project->syntaxTemplate());
//        codeEdit->setHighlightList(document, project->projectHighlightList());

        SyntaxTemplateLoader::CompletionDataList completionDataList;

        QList<ProjectInterface::CompleterInfo> completerInfoList = project->completerInfoList();

        foreach(const ProjectInterface::CompleterInfo &completerInfo, completerInfoList)
        {
            SyntaxTemplateLoader::CompletionData completionData;
            completionData.name = completerInfo.name;
            completionData.info = completerInfo.info;
            completionDataList.append(completionData);
        }

        codeEdit->setCompleterItems(document, completionDataList);
    }
}

void ProjectManager::on_treeWidgetItem_doubleClicked(QTreeWidgetItem*, int)
{
    int index = currentFile();
    ProjectInterface *project = currentProject();
    projectFileOpen(project, index);
}

QTreeWidgetItem *ProjectManager::currentProjectItem()
{
    QTreeWidgetItem *item = m_treeWidget->currentItem();

    while(item)
    {
        if(m_itemTypeMap[item] == ITEMTYPE_PROJECT)
            return item;

        item = item->parent();
    }

    return NULL;
}

ProjectInterface* ProjectManager::currentProject()
{
    QTreeWidgetItem *item = currentProjectItem();

    QList<ProjectInterface*> keys = m_projectMap.keys();
    foreach(ProjectInterface *project , keys)
    {
        if(m_projectMap[project].item == item)
            return project;
    }

    return NULL;
}

QTreeWidgetItem *ProjectManager::currentCategoryItem()
{
    QTreeWidgetItem *item = m_treeWidget->currentItem();

    while(item)
    {
        if(m_itemTypeMap[item] == ITEMTYPE_CATEGORY)
            return item;

        item = item->parent();
    }

    return NULL;
}

QString ProjectManager::currentCategory()
{
    ProjectInterface *project  = currentProject();

    if(!project)
        return QString();

    QTreeWidgetItem *item = currentCategoryItem();

    if(!item)
        return QString();

    QList<QString> keys = m_projectMap[project].categoryMap.keys();
    foreach(const QString &key, keys)
    {
        if(m_projectMap[project].categoryMap[key] == item)
            return key;
    }

    return QString();
}

QTreeWidgetItem *ProjectManager::currentFileItem()
{
    QTreeWidgetItem *item = m_treeWidget->currentItem();

    if(!item)
        return NULL;

    if(m_itemTypeMap[item] != ITEMTYPE_FILE)
        return NULL;

    return item;
}

int ProjectManager::currentFile()
{
    ProjectInterface *project  = currentProject();

    if(!project)
        return - 1;

    QTreeWidgetItem *item = currentFileItem();

    if(!item)
        return -1;

    QList<int> indexList = m_projectMap[project].fileMap.keys();
    foreach(const int &index, indexList)
    {
        if(m_projectMap[project].fileMap[index] == item)
            return index;
    }

    return -1;
}

void ProjectManager::on_project_fileAdded(int index)
{
    if(index < 0)
        return;

    ProjectInterface *project = dynamic_cast<ProjectInterface*>(sender());

    if(!project)
      return;

    QString type = project->projectFileType(index);
    QString path = project->projectFilePath(index);

    QTreeWidgetItem *fileItem = new QTreeWidgetItem;
    m_itemTypeMap[fileItem] = ITEMTYPE_FILE;

    fileItem->setText(0, path);

    QTreeWidgetItem *categoryItem = m_projectMap[project].categoryMap[type];

    if(!categoryItem)
      return;

    categoryItem->addChild(fileItem);

    m_projectMap[project].fileMap[index] = fileItem;
}

void ProjectManager::on_project_fileRemovedPre(int index)
{
  ProjectInterface *project  = dynamic_cast<ProjectInterface*>(sender());

  if(!project)
      return;

  QTreeWidgetItem *fileItem = m_projectMap[project].fileMap[index];

  delete fileItem;
}

void ProjectManager::on_project_loading()
{
    ProjectInterface *project  = dynamic_cast<ProjectInterface*>(sender());

    if(!project)
        return;

    disconnect(project, SIGNAL(projectLoading()), this, SLOT(on_project_loading()));

    QTreeWidgetItem *itemProject = new QTreeWidgetItem;
    m_itemTypeMap[itemProject] = ITEMTYPE_PROJECT;

    itemProject->setText(0, project->projectName());

    QStringList fileTypeList = project->fileTypeList();
    foreach(const QString &type, fileTypeList)
    {
        if(type.isEmpty())
            continue;

        QTreeWidgetItem *itemCategory = new QTreeWidgetItem;
        m_itemTypeMap[itemCategory] = ITEMTYPE_CATEGORY;
        m_projectMap[project].categoryMap[type] = itemCategory;

        itemCategory->setText(0, type);
        itemCategory->setExpanded(true);
        itemProject->addChild(itemCategory);
    }

    m_projectMap[project].item = itemProject;
    m_treeWidget->addTopLevelItem(itemProject);
    m_treeWidget->expandAll();

    connect(project, SIGNAL(projectFileAdded(int)), this, SLOT(on_project_fileAdded(int)));
    connect(project, SIGNAL(projectFileRemovedPre(int)), this, SLOT(on_project_fileRemovedPre(int)));
}

void ProjectManager::on_project_loaded()
{
    ProjectInterface *project  = dynamic_cast<ProjectInterface*>(sender());

    if(!project)
        return;

    disconnect(project, SIGNAL(projectLoaded()), this, SLOT(on_project_loaded()));

    projectFileOpen(project, project->projectMainFile());

    Logger::write("Project " + project->projectName() + " loaded.");
}

void ProjectManager::on_fileEdit_fileSaved(QString path)
{
    QList<ProjectInterface*> projectList = m_projectMap.keys();

    ProjectInterface *project = NULL;

    foreach(ProjectInterface *_project, projectList)
    {
        int index = _project->projectFileIndex(path);

        if(index >= 0)
        {
            _project->projectFileInfoUpdateRequest(index);
            project = _project;
        }
    }

    if(!project)
        return;

    foreach(const QString &path, project->projectFileList("", true))
    {
        int fileIndex = m_fileEdit->getFileIndex(path);

        if(fileIndex < 0)
            continue;

        QTextDocument *textDocument = m_fileEdit->documentAt(fileIndex);

        if(!textDocument)
            continue;

        SyntaxTemplateLoader::CompletionDataList completionDataList;
        QList<ProjectInterface::CompleterInfo> completerInfoList = project->completerInfoList();

        foreach(const ProjectInterface::CompleterInfo &completerInfo, completerInfoList)
        {
            SyntaxTemplateLoader::CompletionData completionData;
            completionData.name = completerInfo.name;
            completionData.info = completerInfo.info;
            completionDataList.append(completionData);
        }

        m_fileEdit->codeEdit()->setCompleterItems(textDocument, completionDataList);
    }
}

void ProjectManager::on_project_buildStarted()
{
    if(m_progressBar)
    {
        m_progressBar->setValue(0);
        m_progressBar->show();
    }

    m_issueInfoItemMap.clear();
    m_buildIssuesWidget->clear();
    m_buildOutputWidget->clear();

    isBuilding = true;
}

void ProjectManager::on_issueWidget_itemClicked(QListWidgetItem *item)
{
    if(!item)
        return;

    if(!m_issueInfoItemMap.contains(item))
        return;

    ProjectInterface::IssueInfo issueInfo = m_issueInfoItemMap[item];

    m_fileEdit->fileOpen(issueInfo.filePath);

    int line = issueInfo.line;

    if(line < 0 || line > m_fileEdit->document()->blockCount())
        return;

    QTextCursor textCursor(m_fileEdit->document()->findBlockByLineNumber(line - 1));
    m_fileEdit->codeEdit()->setTextCursor(textCursor);
}

void ProjectManager::on_project_buildFinished()
{
    ProjectInterface *project  = dynamic_cast<ProjectInterface*>(sender());

    if(m_progressBar)
    {
        m_progressBar->setValue(0);
        m_progressBar->hide();
    }

    if(project)
    {
        QList<ProjectInterface::IssueInfo> issueInfoList = project->projectBuildIssues();

        m_buildOutputWidget->setPlainText(project->projectBuildOutput());

        if(issueInfoList.size())
            m_buildIssuesWidget->show();

        foreach(const ProjectInterface::IssueInfo &issueInfo, issueInfoList)
        {
            QListWidgetItem *item = new QListWidgetItem;

            QColor backgroundColor;
            QColor foregroundColor;

            switch(issueInfo.type)
            {
            case ProjectInterface::ISSUE_WARNING:
                backgroundColor = m_skinLoader->editorFormat("ISSUE_WARNING").background().color();
                foregroundColor = m_skinLoader->editorFormat("ISSUE_WARNING").foreground().color();
                break;
            case ProjectInterface::ISSUE_ERROR:
                backgroundColor = m_skinLoader->editorFormat("ISSUE_ERROR").background().color();
                foregroundColor = m_skinLoader->editorFormat("ISSUE_ERROR").foreground().color();
                break;
            case ProjectInterface::ISSUE_FATAL_ERROR:
                backgroundColor = m_skinLoader->editorFormat("ISSUE_FATAL_ERROR").background().color();
                foregroundColor = m_skinLoader->editorFormat("ISSUE_FATAL_ERROR").foreground().color();
                break;
            }

            if(backgroundColor.isValid())
                item->setBackground(backgroundColor);

            if(foregroundColor.isValid())
                item->setForeground(foregroundColor);

            item->setText(issueInfo.errorString);
            m_issueInfoItemMap[item] = issueInfo;
            m_buildIssuesWidget->addItem(item);
        }
    }

    /*if(m_buildOutputArea && project)
    {

        QLayout *layout = m_buildOutputArea->layout();

        while(layout->count() > 0)
        {
            QLayoutItem* item = layout->takeAt(0);

            if(item)
                item->widget()->setParent(0);
        }

        QWidget *widget = project->buildOutputWidget();

        if(widget)
            layout->addWidget(widget);
    }*/

    isBuilding = false;
}

void ProjectManager::on_project_buildStatus(int current, int total)
{
    if(m_progressBar)
    {
        m_progressBar->setMaximum(total);
        m_progressBar->setValue(current);
    }
}

void ProjectManager::on_treeWidgetItem_customContextMenuRequested(QPoint point)
{
    QTreeWidgetItem *item = m_treeWidget->itemAt(point);

    if(!item)
        return;

    QMenu menu("", m_treeWidget);

    switch(m_itemTypeMap[item])
    {
    case ITEMTYPE_PROJECT:
        menu.addAction(m_actProjectClose);
        menu.addAction(m_actProjectBuild);
        menu.addAction(m_actProjectBuildDebug);
        menu.addAction(m_actProjectRun);
        menu.addAction(m_actProjectRunDebug);
    case ITEMTYPE_CATEGORY:
        menu.addAction(m_actProjectFileNew);
        menu.addAction(m_actProjectFileAdd);
        break;
    case ITEMTYPE_FILE:
        menu.addAction(m_actProjectFileRemove);
        menu.addAction(m_actProjectFileRename);
        menu.addAction(m_actProjectFileSetMain);
        break;
    default:
        return;
    }

    menu.exec(m_treeWidget->viewport()->mapToGlobal(point));
}

void ProjectManager::projectFileSaveAll(ProjectInterface *project)
{
    if(!project)
        return;

    foreach(const QString &type, project->fileTypeList())
    {
        foreach(const QString &path, project->projectFileList(type, true))
        {
            int index = m_fileEdit->getFileIndex(path);
            m_fileEdit->fileSave(index);
        }
    }
}
