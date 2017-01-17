#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>

#include <QFileDialog>

#include <QBoxLayout>
#include <QSplitter>
#include <QTreeWidget>
#include <QLabel>
#include <QAction>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "projectinterface.h"
#include "syntaxhighlighter.h"

#include <QDebug>
#include <QMimeData>

#include "logger.h"

#define DIR_ICONS   "icons"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_syntaxTemplateLoader = SyntaxTemplateLoader::instance();
    m_projectPluginLoader = ProjectPluginLoader::instance();
    m_settingLoader = SettingLoader::instance();
    m_skinLoader = SkinLoader::instance();
    m_fileEdit = new FileEdit;
    m_settingWindow = new SettingWindow;

    m_buildingProgressBar = new QProgressBar;
    m_buildingProgressBar->setTextVisible(false);
    ui->statusBar->addWidget(m_buildingProgressBar, 1);
    m_buildingProgressBar->hide();

    m_buildIssuesWidget = new QListWidget;

    m_buildOutputWidget = new QPlainTextEdit;
    m_buildOutputWidget->setReadOnly(true);

    m_buildIssuesButton = new QPushButton("Issues");
    m_buildOutputButton = new QPushButton("Output");

    m_buildPaneLayout = new QHBoxLayout;
    m_buildPaneLayout->setMargin(0);
    m_buildPaneLayout->addWidget(m_buildIssuesButton);
    m_buildPaneLayout->addWidget(m_buildOutputButton);

    m_buildPanel = new QWidget;
    m_buildPanel->setLayout(m_buildPaneLayout);
    m_buildPanel->setContentsMargins(0, 0, 0, 0);
    m_buildPanel->setFixedHeight(24);

    m_buildAreaSplitter = new QSplitter;
    m_buildAreaSplitter->setOrientation(Qt::Vertical);
    m_buildAreaSplitter->setLineWidth(0);
    m_buildAreaSplitter->setContentsMargins(8, 8, 8 ,8);
    m_buildAreaSplitter->insertWidget(0, ui->editorArea);
    m_buildAreaSplitter->insertWidget(1, m_buildIssuesWidget);
    m_buildAreaSplitter->insertWidget(2, m_buildOutputWidget);
    m_buildAreaSplitter->insertWidget(3, m_buildPanel);
    m_buildAreaSplitter->setStretchFactor(0, 1);
    m_buildAreaSplitter->setStretchFactor(1, 0);
    m_buildAreaSplitter->setStretchFactor(2, 0);
    m_buildAreaSplitter->setSizes(QList<int>()<<512<<128<<128<<24);
    m_buildAreaSplitter->setChildrenCollapsible(false);

    ui->editorArea->layout()->addWidget(m_fileEdit);
    m_splitter = new QSplitter;
    m_splitter->setLineWidth(0);
    m_splitter->setContentsMargins(0, 0, 0 ,0);
    m_splitter->insertWidget(0, ui->treeWidget);
    m_splitter->insertWidget(1, m_buildAreaSplitter);
    m_splitter->setStretchFactor(0, 0);
    m_splitter->setStretchFactor(1, 1);
    m_splitter->setSizes(QList<int>()<<192<<512);

    ui->centralWidget->layout()->addWidget(m_splitter);

    ui->treeWidget->sortItems(0, Qt::AscendingOrder);

    m_projectManager = new ProjectManager(ui->treeWidget, m_fileEdit);
    m_projectManager->setProgressBar(m_buildingProgressBar);
    m_projectManager->setBuildIssuesWidget(m_buildIssuesWidget);
    m_projectManager->setBuildOutputWidget(m_buildOutputWidget);
    m_projectManager->setMenu(ui->menuProject);

    QToolBar *toolBar = new QToolBar;
    toolBar->setIconSize(QSize(24, 24));
    m_projectManager->setToolBar(toolBar);

    ui->toolBar->setIconSize(QSize(24, 24));
    ui->toolBar->addAction(ui->actFileNew);
    ui->toolBar->addAction(ui->actFileOpen);
    ui->toolBar->addAction(ui->actFileSave);
    ui->toolBar->addAction(ui->actFileClose);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actUndo);
    ui->toolBar->addAction(ui->actRedo);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(toolBar);

    m_findDialog = new FindDialog(m_fileEdit->codeEdit());
    m_gotoDialog = new GoToDialog(m_fileEdit->codeEdit());

    connect(ui->actFileNew, SIGNAL(triggered()), m_fileEdit, SLOT(fileNew()));
    //connect(ui->actFileOpen, SIGNAL(triggered()), m_fileEdit, SLOT(fileOpen()));
    connect(ui->actFileSave, SIGNAL(triggered()), m_fileEdit, SLOT(fileSave()));
    connect(ui->actFileSaveAs, SIGNAL(triggered()), m_fileEdit, SLOT(fileSaveAs()));
    connect(ui->actFileClose, SIGNAL(triggered()), m_fileEdit, SLOT(fileClose()));
    connect(ui->actFileCloseAll, SIGNAL(triggered()), m_fileEdit, SLOT(fileCloseAll()));

    connect(ui->actUndo, SIGNAL(triggered()), m_fileEdit->codeEdit(), SLOT(undo()));
    connect(ui->actRedo, SIGNAL(triggered()), m_fileEdit->codeEdit(), SLOT(redo()));

    connect(ui->actCut, SIGNAL(triggered()), m_fileEdit->codeEdit(), SLOT(cut()));
    connect(ui->actCopy, SIGNAL(triggered()), m_fileEdit->codeEdit(), SLOT(copy()));
    connect(ui->actPaste, SIGNAL(triggered()), m_fileEdit->codeEdit(), SLOT(paste()));

    connect(ui->actSelectAll, SIGNAL(triggered()), m_fileEdit->codeEdit(), SLOT(selectAll()));

    connect(ui->actFind, SIGNAL(triggered()), m_findDialog, SLOT(show()));
    connect(ui->actFindNext, SIGNAL(triggered()), m_findDialog, SLOT(find()));
    connect(ui->actGoTo, SIGNAL(triggered()), m_gotoDialog, SLOT(show()));

    connect(ui->actOptions, SIGNAL(triggered()), m_settingWindow, SLOT(show()));

    connect(ui->actProjectClose, SIGNAL(triggered()), m_projectManager, SLOT(projectClose()));

    connect(m_skinLoader, SIGNAL(loaded()), this, SLOT(on_skinLoader_loaded()));
    connect(m_buildIssuesButton, SIGNAL(clicked()), this, SLOT(on_buildIssuesButton_clicked()));
    connect(m_buildOutputButton, SIGNAL(clicked()), this, SLOT(on_buildOutputButton_clicked()));

    highlightTemplateListLoad();
    projectPluginListLoad();
    recentProjectListLoad();
    loadSize();

    m_skinLoader->load(m_settingLoader->skin());

    m_buildIssuesWidget->hide();
    m_buildOutputWidget->hide();

    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete m_fileEdit;
    delete m_projectManager;

    delete m_findDialog;
    delete m_gotoDialog;
    delete m_settingWindow;
    delete m_buildIssuesButton;
    delete m_buildOutputButton;
    //delete m_editorAreaLayout;
    //delete m_buildOutputAreaLayout;
    delete m_buildIssuesWidget;
    delete m_buildAreaSplitter;
    delete m_buildingProgressBar;
    delete m_splitter;

    delete ui;

    Logger::write("Main window closed.");
}

void MainWindow::fileOpen(QString path)
{
    if(path.isEmpty())
        return;

    int size = path.size();
    int index = path.lastIndexOf('.') + 1;
    QString extension = path.right(size - index);

    foreach(const QString &projectName, m_projectPluginLoader->projectList())
    {
        ProjectInterface *instance = m_projectPluginLoader->projectInstance(projectName);

        if(instance->extension() == extension)
        {
            ProjectInterface *project = m_projectPluginLoader->projectLoad(projectName);
            m_projectManager->projectOpen(project, path);

            recentProjectAdd(path, projectName);
            recentProjectListSave();

            return;
        }
    }

    m_fileEdit->fileOpen(path);
}

//Highlight templates

void MainWindow::highlightTemplateListLoad()
{
    highlightTemplateAdd("None");

    foreach(const QString &name, m_syntaxTemplateLoader->templateList())
        highlightTemplateAdd(name);

    Logger::write("Highligh templates loaded.");
}

void MainWindow::highlightTemplateAdd(QString name)
{
    m_highlightTemplateList.append(name);

    QAction *actSetHighlightTemplate = new QAction(name, this);
    ui->menuSet_Highlighter->addAction(actSetHighlightTemplate);
    connect(actSetHighlightTemplate, SIGNAL(triggered()), this, SLOT(on_actSetHighlightTemplate_triggered()));
}

//Project plugins

void MainWindow::projectPluginListLoad()
{
    foreach(const QString &projectName, m_projectPluginLoader->projectList())
        projectPluginAdd(projectName);

    Logger::write("Project plugin list loaded.");
}

void MainWindow::projectPluginAdd(QString projectName)
{
    QString iconPath = QString(DIR_ICONS) + "/projects/"+ projectName + ".png";
    bool isIconExists = QFile(iconPath).exists();

    QAction *actProjectNew = new QAction(projectName, this);

    if(isIconExists)
        actProjectNew->setIcon(QIcon(iconPath));

    ui->menuProjectNew->addAction(actProjectNew);
    connect(actProjectNew, SIGNAL(triggered()), this, SLOT(on_actProjectNew_triggered()));

    if(m_projectPluginLoader->projectInstance(projectName)->isCompileSupported())
    {
        QAction *actFileCompile = new QAction(projectName, this);

        if(isIconExists)
            actFileCompile->setIcon(QIcon(iconPath));

        ui->menuFileCompile->addAction(actFileCompile);
        connect(actFileCompile, SIGNAL(triggered()), this, SLOT(on_actFileCompile_triggered()));
    }
}

//Recent project

void MainWindow::recentProjectListLoad()
{
    QFile file("configs/recentprojects.json");

    if(!file.open(QFile::ReadOnly))
        return;

    QByteArray byteArray = file.readAll();
    file.close();

    QJsonDocument jDocument = QJsonDocument::fromJson(byteArray);
    QJsonObject jObject = jDocument.object();

    foreach(const QString &key, jObject.keys())
    {
        QString project = jObject[key].toString();
        recentProjectAdd(key, project);
    }

    Logger::write("Recent project list loaded.");
}

void MainWindow::recentProjectListSave()
{
    if(!m_recentProjectMap.count())
        return;

    QJsonDocument jDocument;
    QJsonObject jObject;

    QStringList keyList = m_recentProjectMap.keys();

    int firstIndex = keyList.size() - 1;
    int lastIndex = keyList.size() - 10;

    if(lastIndex < 0)
        lastIndex = 0;

    for(int i = firstIndex; i >= lastIndex; i--)
    {
        QString key = keyList[i];

        if(key.isEmpty())
            continue;

        jObject[key] = m_recentProjectMap[key];
    }

    jDocument.setObject(jObject);

    QFile file("configs/recentprojects.json");

    if(!file.open(QFile::WriteOnly))
        return;

    file.write(jDocument.toJson());
    file.close();
}

void MainWindow::recentProjectAdd(QString path, QString projectName)
{
    if(path.isEmpty())
        return;

    if(projectName.isEmpty())
        return;

    QString iconPath = QString(DIR_ICONS) + "/projects/"+ projectName + ".png";
    bool isIconExists = QFile(iconPath).exists();

    m_recentProjectMap[path] = projectName;
    QAction *actProjectOpenRecent = new QAction(path, this);

    if(isIconExists)
        actProjectOpenRecent->setIcon(QIcon(iconPath));

    actProjectOpenRecent->setToolTip(path);
    connect(actProjectOpenRecent, SIGNAL(triggered()), this, SLOT(on_actRecentProjectOpen_triggered()));
    ui->menuProjectRecent->addAction(actProjectOpenRecent);
}


//Actions

void MainWindow::on_skinLoader_loaded()
{
    this->setStyleSheet(m_skinLoader->styleSheet());
    m_settingWindow->setStyleSheet(m_skinLoader->styleSheet());
    m_findDialog->setStyleSheet(m_skinLoader->styleSheet());
    m_gotoDialog->setStyleSheet(m_skinLoader->styleSheet());

    ui->actFileNew->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/filenew.png"));
    ui->actFileOpen->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/fileopen.png"));
    ui->actFileSave->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/save.png"));
    ui->actFileSaveAs->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/save.png"));
    ui->actFileClose->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/close.png"));
    ui->actFileCloseAll->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/close.png"));

    ui->actProjectClose->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/projectclose.png"));

    ui->actCopy->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/copy.png"));
    ui->actUndo->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/undo.png"));
    ui->actRedo->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/redo.png"));
    ui->actFind->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/search.png"));

    ui->actOptions->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/options.png"));
    ui->actAbout->setIcon(QIcon(":/icons/" + m_skinLoader->iconsPack() + "/about.png"));
}

void MainWindow::on_actRecentProjectOpen_triggered()
{
    QAction *action = dynamic_cast<QAction*>(sender());

    if(!action)
        return;

    QString path = action->text();
    QString projectName = m_recentProjectMap[path];

    ProjectInterface *project = m_projectPluginLoader->projectLoad(projectName);
    m_projectManager->projectOpen(project, path);
}

void MainWindow::on_actProjectNew_triggered()
{
    QAction *action = dynamic_cast<QAction*>(sender());

    if(!action)
        return;

    QString projectName = action->text();

    ProjectInterface *project = m_projectPluginLoader->projectLoad(projectName);
    m_projectManager->projectNew(project);
}

void MainWindow::on_actProjectOpen_triggered()
{
    QAction *action = dynamic_cast<QAction*>(sender());

    if(!action)
        return;

    QString projectName = action->text();

    ProjectInterface *project = m_projectPluginLoader->projectLoad(projectName);
    QString path = m_projectManager->projectOpen(project);

    if(path.isEmpty())
        return;

    recentProjectAdd(path, projectName);
    recentProjectListSave();
}

void MainWindow::on_actFileCompile_triggered()
{
    QAction *action = dynamic_cast<QAction*>(sender());

    if(!action)
        return;

    QString projectName = action->text();

    ProjectInterface *instance = m_projectPluginLoader->projectInstance(projectName);
    int index = m_fileEdit->index();
    QString path = m_fileEdit->filePath(index);

    if(path.isEmpty())
        return;

    instance->compile(path);
}

void MainWindow::on_actSetHighlightTemplate_triggered()
{
    QAction *action = dynamic_cast<QAction*>(sender());

    if(!action)
        return;

    QString templateName = action->text();

    if(templateName == "None")
        templateName.clear();

    QTextDocument *textDocument = m_fileEdit->codeEdit()->document();
    m_fileEdit->codeEdit()->setSyntaxTemplate(textDocument, action->text());
}

void MainWindow::on_actAbout_triggered()
{
    QLabel *label = new QLabel;
    label->setText("Hello,\n"
                   "It's just a test Fanxy Studio version.\n\n"
                   "--------------------------------\n"
                   "Copyright © 2015 by Hedgehog Fog");

    label->setAlignment(Qt::AlignCenter);
    label->setFixedSize(320, 160);
    label->setStyleSheet(m_skinLoader->styleSheet());

    label->show();
}

void MainWindow::on_actFileOpen_triggered()
{
    //Setup filter
    QString filters = "All types (*.*)";

    QStringList projectList = m_projectPluginLoader->projectList();
    foreach(const QString &projectName, projectList)
    {
        QString extension = m_projectPluginLoader->projectInstance(projectName)->extension();
        filters += ";;" + projectName + " (*." + extension + ")";
    }

    //Open file dialog
    QString path = QFileDialog::getOpenFileName
    (
        this,
        "Open file...",
        QString(),
        filters.toUtf8()
    );

    fileOpen(path);
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData *mimeData = event->mimeData();

    if (!mimeData->hasUrls())
        return;

    QList<QUrl> urlList = mimeData->urls();
    for(int i = 0; i < urlList.size(); i++)
    {
        QString path = urlList.at(i).toLocalFile();
        fileOpen(path);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_fileEdit->fileCloseAll();

    saveSize();

    event->accept();
}

void MainWindow::loadSize()
{
    QRect screenRect = QApplication::desktop()->screenGeometry();
    QRect rect = this->geometry();

    int screenWidth = screenRect.width();
    int screenHeigth = screenRect.height();

    int width = m_settingLoader->windowWidth();
    int height = m_settingLoader->windowHeight();

    if(width <= 0 || width > screenWidth)
        width = rect.width();

    if(height <= 0 || height > screenHeigth)
        height = rect.height();

    int x = (screenWidth - width)/2;
    int y = (screenHeigth - height)/2;

    this->setGeometry(x, y, width, height);

    if(m_settingLoader->windowIsMaximized())
        this->showMaximized();
}

void MainWindow::saveSize()
{
    QByteArray byteArray;

    QFile file(FILE_CONFIG_SETTINGS);

    if(!file.open(QFile::ReadOnly))
        return;

    byteArray = file.readAll();
    file.close();

    if(byteArray.isEmpty())
        return;

    QJsonObject jObject = QJsonDocument::fromJson(byteArray).object();
    jObject["WindowWidth"] = this->width();
    jObject["WindowHeight"] = this->height();
    jObject["WindowIsMaximized"] = this->isMaximized();

    if(!file.open(QFile::WriteOnly))
        return;

    file.write(QJsonDocument(jObject).toJson());
    file.close();
}

void MainWindow::on_buildIssuesButton_clicked()
{
    m_buildOutputWidget->hide();

    if(m_buildIssuesWidget->isVisible())
        m_buildIssuesWidget->hide();
    else
        m_buildIssuesWidget->show();
}

void MainWindow::on_buildOutputButton_clicked()
{
    m_buildIssuesWidget->hide();

    if(m_buildOutputWidget->isVisible())
        m_buildOutputWidget->hide();
    else
        m_buildOutputWidget->show();
}
