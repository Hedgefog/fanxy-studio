/********************************************************************************
** Form generated from reading UI file 'createprojectdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEPROJECTDIALOG_H
#define UI_CREATEPROJECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CreateProjectDialog
{
public:
    QLabel *workspaceLabel;
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QPushButton *workspaceBrowserButton;
    QPushButton *acceptButton;
    QLabel *infoLabel;
    QLineEdit *workspaceEdit;

    void setupUi(QDialog *CreateProjectDialog)
    {
        if (CreateProjectDialog->objectName().isEmpty())
            CreateProjectDialog->setObjectName(QStringLiteral("CreateProjectDialog"));
        CreateProjectDialog->resize(336, 220);
        workspaceLabel = new QLabel(CreateProjectDialog);
        workspaceLabel->setObjectName(QStringLiteral("workspaceLabel"));
        workspaceLabel->setGeometry(QRect(8, 104, 320, 24));
        QFont font;
        font.setPointSize(10);
        workspaceLabel->setFont(font);
        nameLabel = new QLabel(CreateProjectDialog);
        nameLabel->setObjectName(QStringLiteral("nameLabel"));
        nameLabel->setGeometry(QRect(8, 48, 320, 24));
        nameLabel->setFont(font);
        nameEdit = new QLineEdit(CreateProjectDialog);
        nameEdit->setObjectName(QStringLiteral("nameEdit"));
        nameEdit->setGeometry(QRect(8, 72, 320, 24));
        workspaceBrowserButton = new QPushButton(CreateProjectDialog);
        workspaceBrowserButton->setObjectName(QStringLiteral("workspaceBrowserButton"));
        workspaceBrowserButton->setGeometry(QRect(264, 128, 64, 24));
        workspaceBrowserButton->setAutoDefault(false);
        acceptButton = new QPushButton(CreateProjectDialog);
        acceptButton->setObjectName(QStringLiteral("acceptButton"));
        acceptButton->setGeometry(QRect(200, 184, 128, 24));
        infoLabel = new QLabel(CreateProjectDialog);
        infoLabel->setObjectName(QStringLiteral("infoLabel"));
        infoLabel->setGeometry(QRect(0, 0, 336, 48));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        infoLabel->setFont(font1);
        infoLabel->setAlignment(Qt::AlignCenter);
        workspaceEdit = new QLineEdit(CreateProjectDialog);
        workspaceEdit->setObjectName(QStringLiteral("workspaceEdit"));
        workspaceEdit->setGeometry(QRect(8, 128, 256, 24));

        retranslateUi(CreateProjectDialog);

        QMetaObject::connectSlotsByName(CreateProjectDialog);
    } // setupUi

    void retranslateUi(QDialog *CreateProjectDialog)
    {
        CreateProjectDialog->setWindowTitle(QApplication::translate("CreateProjectDialog", "Create Project", Q_NULLPTR));
        workspaceLabel->setText(QApplication::translate("CreateProjectDialog", "Workspace", Q_NULLPTR));
        nameLabel->setText(QApplication::translate("CreateProjectDialog", "Name", Q_NULLPTR));
        nameEdit->setText(QString());
        workspaceBrowserButton->setText(QApplication::translate("CreateProjectDialog", "Browser", Q_NULLPTR));
        acceptButton->setText(QApplication::translate("CreateProjectDialog", "Create", Q_NULLPTR));
        infoLabel->setText(QApplication::translate("CreateProjectDialog", "Create new project", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CreateProjectDialog: public Ui_CreateProjectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEPROJECTDIALOG_H
