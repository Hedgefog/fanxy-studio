/********************************************************************************
** Form generated from reading UI file 'setupwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPWIDGET_H
#define UI_SETUPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetupWidget
{
public:
    QPushButton *applyButton;
    QLabel *gamePathLabel;
    QPushButton *gamePathBrowserButton;
    QLineEdit *gamePathEdit;
    QLabel *executeCmdLabel;
    QLineEdit *executeCmdEdit;
    QLabel *gameModLabel;
    QLineEdit *gameModEdit;
    QLabel *compilerLabel;
    QPushButton *compilerBrowserButton;
    QLineEdit *compilerEdit;
    QLineEdit *outputPathEdit;
    QLabel *outputPathLabel;
    QPushButton *outputPathBrowserButton;

    void setupUi(QWidget *SetupWidget)
    {
        if (SetupWidget->objectName().isEmpty())
            SetupWidget->setObjectName(QStringLiteral("SetupWidget"));
        SetupWidget->resize(244, 304);
        applyButton = new QPushButton(SetupWidget);
        applyButton->setObjectName(QStringLiteral("applyButton"));
        applyButton->setGeometry(QRect(108, 272, 128, 24));
        gamePathLabel = new QLabel(SetupWidget);
        gamePathLabel->setObjectName(QStringLiteral("gamePathLabel"));
        gamePathLabel->setGeometry(QRect(8, 56, 224, 24));
        QFont font;
        font.setPointSize(10);
        gamePathLabel->setFont(font);
        gamePathBrowserButton = new QPushButton(SetupWidget);
        gamePathBrowserButton->setObjectName(QStringLiteral("gamePathBrowserButton"));
        gamePathBrowserButton->setGeometry(QRect(168, 80, 64, 24));
        gamePathEdit = new QLineEdit(SetupWidget);
        gamePathEdit->setObjectName(QStringLiteral("gamePathEdit"));
        gamePathEdit->setGeometry(QRect(8, 80, 160, 24));
        executeCmdLabel = new QLabel(SetupWidget);
        executeCmdLabel->setObjectName(QStringLiteral("executeCmdLabel"));
        executeCmdLabel->setGeometry(QRect(8, 200, 224, 24));
        executeCmdEdit = new QLineEdit(SetupWidget);
        executeCmdEdit->setObjectName(QStringLiteral("executeCmdEdit"));
        executeCmdEdit->setGeometry(QRect(8, 224, 224, 24));
        gameModLabel = new QLabel(SetupWidget);
        gameModLabel->setObjectName(QStringLiteral("gameModLabel"));
        gameModLabel->setGeometry(QRect(8, 152, 224, 24));
        gameModLabel->setFont(font);
        gameModEdit = new QLineEdit(SetupWidget);
        gameModEdit->setObjectName(QStringLiteral("gameModEdit"));
        gameModEdit->setGeometry(QRect(8, 176, 224, 24));
        compilerLabel = new QLabel(SetupWidget);
        compilerLabel->setObjectName(QStringLiteral("compilerLabel"));
        compilerLabel->setGeometry(QRect(8, 8, 224, 24));
        compilerLabel->setFont(font);
        compilerBrowserButton = new QPushButton(SetupWidget);
        compilerBrowserButton->setObjectName(QStringLiteral("compilerBrowserButton"));
        compilerBrowserButton->setGeometry(QRect(168, 32, 64, 24));
        compilerEdit = new QLineEdit(SetupWidget);
        compilerEdit->setObjectName(QStringLiteral("compilerEdit"));
        compilerEdit->setGeometry(QRect(8, 32, 160, 24));
        outputPathEdit = new QLineEdit(SetupWidget);
        outputPathEdit->setObjectName(QStringLiteral("outputPathEdit"));
        outputPathEdit->setGeometry(QRect(8, 128, 160, 24));
        outputPathLabel = new QLabel(SetupWidget);
        outputPathLabel->setObjectName(QStringLiteral("outputPathLabel"));
        outputPathLabel->setGeometry(QRect(8, 104, 224, 24));
        outputPathLabel->setFont(font);
        outputPathBrowserButton = new QPushButton(SetupWidget);
        outputPathBrowserButton->setObjectName(QStringLiteral("outputPathBrowserButton"));
        outputPathBrowserButton->setGeometry(QRect(168, 128, 64, 24));

        retranslateUi(SetupWidget);

        QMetaObject::connectSlotsByName(SetupWidget);
    } // setupUi

    void retranslateUi(QWidget *SetupWidget)
    {
        SetupWidget->setWindowTitle(QApplication::translate("SetupWidget", "Form", Q_NULLPTR));
        applyButton->setText(QApplication::translate("SetupWidget", "Apply", Q_NULLPTR));
        gamePathLabel->setText(QApplication::translate("SetupWidget", "Game path (Half-Life)", Q_NULLPTR));
        gamePathBrowserButton->setText(QApplication::translate("SetupWidget", "Browser", Q_NULLPTR));
        executeCmdLabel->setText(QApplication::translate("SetupWidget", "Execute command:", Q_NULLPTR));
        gameModLabel->setText(QApplication::translate("SetupWidget", "Mod:", Q_NULLPTR));
        gameModEdit->setText(QString());
        compilerLabel->setText(QApplication::translate("SetupWidget", "Compiler", Q_NULLPTR));
        compilerBrowserButton->setText(QApplication::translate("SetupWidget", "Browser", Q_NULLPTR));
        outputPathLabel->setText(QApplication::translate("SetupWidget", "Compile output path", Q_NULLPTR));
        outputPathBrowserButton->setText(QApplication::translate("SetupWidget", "Browser", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SetupWidget: public Ui_SetupWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPWIDGET_H
