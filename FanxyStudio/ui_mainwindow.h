/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actFileOpen;
    QAction *actFileClose;
    QAction *actUndo;
    QAction *actRedo;
    QAction *actCopy;
    QAction *actCut;
    QAction *actPaste;
    QAction *actSelectAll;
    QAction *actFileNew;
    QAction *actFind;
    QAction *actGoTo;
    QAction *actProjectClose;
    QAction *actAbout;
    QAction *actFileSave;
    QAction *actOptions;
    QAction *actFileSaveAs;
    QAction *actFileCloseAll;
    QAction *actFindNext;
    QWidget *centralWidget;
    QHBoxLayout *centralLayout;
    QTreeWidget *treeWidget;
    QWidget *editorArea;
    QVBoxLayout *editorAreaLayout;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuProjectNew;
    QMenu *menuProjectRecent;
    QMenu *menuFileCompile;
    QMenu *menuEdit;
    QMenu *menuSearch;
    QMenu *menuHelp;
    QMenu *menuView;
    QMenu *menuSet_Highlighter;
    QMenu *menuProject;
    QMenu *menuTools;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(640, 480);
        MainWindow->setStyleSheet(QStringLiteral(""));
        actFileOpen = new QAction(MainWindow);
        actFileOpen->setObjectName(QStringLiteral("actFileOpen"));
        actFileClose = new QAction(MainWindow);
        actFileClose->setObjectName(QStringLiteral("actFileClose"));
        actUndo = new QAction(MainWindow);
        actUndo->setObjectName(QStringLiteral("actUndo"));
        actRedo = new QAction(MainWindow);
        actRedo->setObjectName(QStringLiteral("actRedo"));
        actCopy = new QAction(MainWindow);
        actCopy->setObjectName(QStringLiteral("actCopy"));
        actCut = new QAction(MainWindow);
        actCut->setObjectName(QStringLiteral("actCut"));
        actPaste = new QAction(MainWindow);
        actPaste->setObjectName(QStringLiteral("actPaste"));
        actSelectAll = new QAction(MainWindow);
        actSelectAll->setObjectName(QStringLiteral("actSelectAll"));
        actFileNew = new QAction(MainWindow);
        actFileNew->setObjectName(QStringLiteral("actFileNew"));
        actFind = new QAction(MainWindow);
        actFind->setObjectName(QStringLiteral("actFind"));
        actGoTo = new QAction(MainWindow);
        actGoTo->setObjectName(QStringLiteral("actGoTo"));
        actProjectClose = new QAction(MainWindow);
        actProjectClose->setObjectName(QStringLiteral("actProjectClose"));
        actAbout = new QAction(MainWindow);
        actAbout->setObjectName(QStringLiteral("actAbout"));
        actFileSave = new QAction(MainWindow);
        actFileSave->setObjectName(QStringLiteral("actFileSave"));
        actOptions = new QAction(MainWindow);
        actOptions->setObjectName(QStringLiteral("actOptions"));
        actFileSaveAs = new QAction(MainWindow);
        actFileSaveAs->setObjectName(QStringLiteral("actFileSaveAs"));
        actFileCloseAll = new QAction(MainWindow);
        actFileCloseAll->setObjectName(QStringLiteral("actFileCloseAll"));
        actFindNext = new QAction(MainWindow);
        actFindNext->setObjectName(QStringLiteral("actFindNext"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralLayout = new QHBoxLayout(centralWidget);
        centralLayout->setSpacing(0);
        centralLayout->setContentsMargins(11, 11, 11, 11);
        centralLayout->setObjectName(QStringLiteral("centralLayout"));
        centralLayout->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(centralWidget);
        treeWidget->headerItem()->setText(0, QString());
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setSortingEnabled(true);
        treeWidget->setHeaderHidden(true);
        treeWidget->header()->setVisible(false);

        centralLayout->addWidget(treeWidget);

        editorArea = new QWidget(centralWidget);
        editorArea->setObjectName(QStringLiteral("editorArea"));
        editorAreaLayout = new QVBoxLayout(editorArea);
        editorAreaLayout->setSpacing(0);
        editorAreaLayout->setContentsMargins(11, 11, 11, 11);
        editorAreaLayout->setObjectName(QStringLiteral("editorAreaLayout"));
        editorAreaLayout->setContentsMargins(0, 0, 0, 0);

        centralLayout->addWidget(editorArea);

        centralLayout->setStretch(0, 160);
        centralLayout->setStretch(1, 512);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setSizeGripEnabled(false);
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuProjectNew = new QMenu(menuFile);
        menuProjectNew->setObjectName(QStringLiteral("menuProjectNew"));
        menuProjectRecent = new QMenu(menuFile);
        menuProjectRecent->setObjectName(QStringLiteral("menuProjectRecent"));
        menuFileCompile = new QMenu(menuFile);
        menuFileCompile->setObjectName(QStringLiteral("menuFileCompile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuSearch = new QMenu(menuBar);
        menuSearch->setObjectName(QStringLiteral("menuSearch"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuSet_Highlighter = new QMenu(menuView);
        menuSet_Highlighter->setObjectName(QStringLiteral("menuSet_Highlighter"));
        menuProject = new QMenu(menuBar);
        menuProject->setObjectName(QStringLiteral("menuProject"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        MainWindow->setMenuBar(menuBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setMovable(false);
        toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuProject->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuSearch->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actFileNew);
        menuFile->addAction(menuProjectNew->menuAction());
        menuFile->addAction(actFileOpen);
        menuFile->addAction(menuProjectRecent->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actFileSave);
        menuFile->addAction(actFileSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actProjectClose);
        menuFile->addSeparator();
        menuFile->addAction(actFileClose);
        menuFile->addAction(actFileCloseAll);
        menuFile->addSeparator();
        menuFile->addAction(menuFileCompile->menuAction());
        menuEdit->addAction(actUndo);
        menuEdit->addAction(actRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actCut);
        menuEdit->addAction(actCopy);
        menuEdit->addAction(actPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actSelectAll);
        menuSearch->addAction(actFind);
        menuSearch->addAction(actFindNext);
        menuSearch->addAction(actGoTo);
        menuHelp->addAction(actAbout);
        menuView->addAction(menuSet_Highlighter->menuAction());
        menuTools->addAction(actOptions);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Fanxy Studio", Q_NULLPTR));
        actFileOpen->setText(QApplication::translate("MainWindow", "&Open File or Project...", Q_NULLPTR));
        actFileOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", Q_NULLPTR));
        actFileClose->setText(QApplication::translate("MainWindow", "&Close", Q_NULLPTR));
        actFileClose->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", Q_NULLPTR));
        actUndo->setText(QApplication::translate("MainWindow", "&Undo", Q_NULLPTR));
        actRedo->setText(QApplication::translate("MainWindow", "&Redo", Q_NULLPTR));
        actCopy->setText(QApplication::translate("MainWindow", "C&opy", Q_NULLPTR));
        actCut->setText(QApplication::translate("MainWindow", "&Cut", Q_NULLPTR));
        actPaste->setText(QApplication::translate("MainWindow", "&Paste", Q_NULLPTR));
        actSelectAll->setText(QApplication::translate("MainWindow", "&Select All", Q_NULLPTR));
        actFileNew->setText(QApplication::translate("MainWindow", "&New File", Q_NULLPTR));
        actFileNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", Q_NULLPTR));
        actFind->setText(QApplication::translate("MainWindow", "&Find", Q_NULLPTR));
        actFind->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", Q_NULLPTR));
        actGoTo->setText(QApplication::translate("MainWindow", "&Go to...", Q_NULLPTR));
        actGoTo->setShortcut(QApplication::translate("MainWindow", "Ctrl+G", Q_NULLPTR));
        actProjectClose->setText(QApplication::translate("MainWindow", "&Close Project", Q_NULLPTR));
        actProjectClose->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+W", Q_NULLPTR));
        actAbout->setText(QApplication::translate("MainWindow", "&About", Q_NULLPTR));
        actFileSave->setText(QApplication::translate("MainWindow", "&Save", Q_NULLPTR));
        actFileSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", Q_NULLPTR));
        actOptions->setText(QApplication::translate("MainWindow", "Options", Q_NULLPTR));
        actFileSaveAs->setText(QApplication::translate("MainWindow", "Save as...", Q_NULLPTR));
        actFileSaveAs->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", Q_NULLPTR));
        actFileCloseAll->setText(QApplication::translate("MainWindow", "Close all", Q_NULLPTR));
        actFindNext->setText(QApplication::translate("MainWindow", "Find Next", Q_NULLPTR));
        actFindNext->setShortcut(QApplication::translate("MainWindow", "F3", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "Fi&le", Q_NULLPTR));
        menuProjectNew->setTitle(QApplication::translate("MainWindow", "&New Project", Q_NULLPTR));
        menuProjectRecent->setTitle(QApplication::translate("MainWindow", "Recent Projects", Q_NULLPTR));
        menuFileCompile->setTitle(QApplication::translate("MainWindow", "Compile as...", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", Q_NULLPTR));
        menuSearch->setTitle(QApplication::translate("MainWindow", "Sea&rch", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("MainWindow", "View", Q_NULLPTR));
        menuSet_Highlighter->setTitle(QApplication::translate("MainWindow", "Set Highlighter", Q_NULLPTR));
        menuProject->setTitle(QApplication::translate("MainWindow", "Project", Q_NULLPTR));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "Tool Bar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
