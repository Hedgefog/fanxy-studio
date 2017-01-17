/********************************************************************************
** Form generated from reading UI file 'finddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDDIALOG_H
#define UI_FINDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FindDialog
{
public:
    QPushButton *findButton;
    QCheckBox *matchwordCheck;
    QPushButton *cancelButton;
    QCheckBox *matchcaseCheck;
    QLineEdit *findEdit;
    QPushButton *replaceButton;
    QLabel *findLabel;
    QWidget *directionWidget;
    QRadioButton *directionUp;
    QRadioButton *directionDown;
    QLabel *directionLabel;
    QLineEdit *replaceEdit;
    QLabel *replaceLabel;
    QPushButton *replaceAllButton;

    void setupUi(QDialog *FindDialog)
    {
        if (FindDialog->objectName().isEmpty())
            FindDialog->setObjectName(QStringLiteral("FindDialog"));
        FindDialog->resize(424, 136);
        findButton = new QPushButton(FindDialog);
        findButton->setObjectName(QStringLiteral("findButton"));
        findButton->setGeometry(QRect(320, 8, 96, 24));
        findButton->setAutoDefault(true);
        matchwordCheck = new QCheckBox(FindDialog);
        matchwordCheck->setObjectName(QStringLiteral("matchwordCheck"));
        matchwordCheck->setGeometry(QRect(10, 96, 174, 24));
        cancelButton = new QPushButton(FindDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(320, 96, 96, 24));
        cancelButton->setAutoDefault(false);
        matchcaseCheck = new QCheckBox(FindDialog);
        matchcaseCheck->setObjectName(QStringLiteral("matchcaseCheck"));
        matchcaseCheck->setGeometry(QRect(10, 72, 174, 24));
        matchcaseCheck->setTristate(false);
        findEdit = new QLineEdit(FindDialog);
        findEdit->setObjectName(QStringLiteral("findEdit"));
        findEdit->setGeometry(QRect(56, 8, 256, 24));
        replaceButton = new QPushButton(FindDialog);
        replaceButton->setObjectName(QStringLiteral("replaceButton"));
        replaceButton->setGeometry(QRect(320, 40, 96, 24));
        replaceButton->setAutoDefault(false);
        findLabel = new QLabel(FindDialog);
        findLabel->setObjectName(QStringLiteral("findLabel"));
        findLabel->setGeometry(QRect(8, 8, 48, 24));
        directionWidget = new QWidget(FindDialog);
        directionWidget->setObjectName(QStringLiteral("directionWidget"));
        directionWidget->setGeometry(QRect(184, 72, 128, 48));
        directionUp = new QRadioButton(directionWidget);
        directionUp->setObjectName(QStringLiteral("directionUp"));
        directionUp->setGeometry(QRect(0, 24, 64, 24));
        directionUp->setChecked(false);
        directionUp->setAutoExclusive(true);
        directionDown = new QRadioButton(directionWidget);
        directionDown->setObjectName(QStringLiteral("directionDown"));
        directionDown->setGeometry(QRect(64, 24, 64, 24));
        directionDown->setChecked(true);
        directionDown->setAutoExclusive(true);
        directionLabel = new QLabel(directionWidget);
        directionLabel->setObjectName(QStringLiteral("directionLabel"));
        directionLabel->setGeometry(QRect(0, 0, 128, 24));
        directionLabel->setIndent(2);
        replaceEdit = new QLineEdit(FindDialog);
        replaceEdit->setObjectName(QStringLiteral("replaceEdit"));
        replaceEdit->setGeometry(QRect(56, 40, 256, 24));
        replaceLabel = new QLabel(FindDialog);
        replaceLabel->setObjectName(QStringLiteral("replaceLabel"));
        replaceLabel->setGeometry(QRect(8, 40, 48, 24));
        replaceAllButton = new QPushButton(FindDialog);
        replaceAllButton->setObjectName(QStringLiteral("replaceAllButton"));
        replaceAllButton->setGeometry(QRect(320, 68, 96, 24));
        replaceAllButton->setAutoDefault(false);

        retranslateUi(FindDialog);

        QMetaObject::connectSlotsByName(FindDialog);
    } // setupUi

    void retranslateUi(QDialog *FindDialog)
    {
        FindDialog->setWindowTitle(QApplication::translate("FindDialog", "Find", Q_NULLPTR));
        findButton->setText(QApplication::translate("FindDialog", "Find", Q_NULLPTR));
        matchwordCheck->setText(QApplication::translate("FindDialog", "Match whole word only", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("FindDialog", "Cancel", Q_NULLPTR));
        matchcaseCheck->setText(QApplication::translate("FindDialog", "Match Case", Q_NULLPTR));
        replaceButton->setText(QApplication::translate("FindDialog", "Replace", Q_NULLPTR));
        findLabel->setText(QApplication::translate("FindDialog", "Find:", Q_NULLPTR));
        directionUp->setText(QApplication::translate("FindDialog", "Up", Q_NULLPTR));
        directionDown->setText(QApplication::translate("FindDialog", "Down", Q_NULLPTR));
        directionLabel->setText(QApplication::translate("FindDialog", "Direction", Q_NULLPTR));
        replaceLabel->setText(QApplication::translate("FindDialog", "Replace:", Q_NULLPTR));
        replaceAllButton->setText(QApplication::translate("FindDialog", "Replace All", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FindDialog: public Ui_FindDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDIALOG_H
