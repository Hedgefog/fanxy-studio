#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "fileedit.h"

#include "skinloader.h"
#include "settingloader.h"
#include "projectpluginloader.h"
#include "syntaxtemplateloader.h"

#include "finddialog.h"
#include "gotodialog.h"

#include "projectmanager.h"
#include "projectinterface.h"
#include "settingwindow.h"

#include <QVBoxLayout>

#include <QSplitter>

#include <QProgressBar>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void fileOpen(QString path);

protected:
    void                    highlightTemplateListLoad();
    void                    highlightTemplateAdd(QString name);

    void                    recentProjectListLoad();
    void                    recentProjectListSave();
    void                    recentProjectAdd(QString path, QString project);

    void                    projectPluginListLoad();
    void                    projectPluginAdd(QString projectName);

    Ui::MainWindow          *ui;
    FileEdit                *m_fileEdit;
    QPlainTextEdit          *m_buildOutputWidget;
    QListWidget             *m_buildIssuesWidget;
    QBoxLayout              *m_buildOutputAreaLayout;
    QWidget                 *m_buildPanel;
    QHBoxLayout             *m_buildPaneLayout;
    FindDialog              *m_findDialog;
    GoToDialog              *m_gotoDialog;
    ProjectManager          *m_projectManager;


    SyntaxTemplateLoader    *m_syntaxTemplateLoader;
    SkinLoader              *m_skinLoader;
    SettingLoader           *m_settingLoader;

    QStringList             m_projectList;
    QSplitter               *m_splitter;
    QSplitter               *m_buildAreaSplitter;
    QPushButton             *m_buildIssuesButton;
    QPushButton             *m_buildOutputButton;
    QProgressBar            *m_buildingProgressBar;

    ProjectPluginLoader     *m_projectPluginLoader;

    SettingWindow           *m_settingWindow;

    QMap<QString, QString>  m_recentProjectMap;
    QStringList             m_highlightTemplateList;

protected slots:
    void                    on_actSetHighlightTemplate_triggered();
    void                    on_actRecentProjectOpen_triggered();
    void                    on_actProjectNew_triggered();
    void                    on_actProjectOpen_triggered();
    void                    on_actAbout_triggered();
    void                    on_actFileCompile_triggered();
    void                    on_actFileOpen_triggered();
    void                    on_skinLoader_loaded();
    void                    on_buildOutputButton_clicked();
    void                    on_buildIssuesButton_clicked();

protected:
    void                    loadSize();
    void                    saveSize();
    void                    dropEvent(QDropEvent* event);
    void                    dragEnterEvent(QDragEnterEvent *event);
    void                    closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
