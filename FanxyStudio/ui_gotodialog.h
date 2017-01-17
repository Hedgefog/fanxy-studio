/********************************************************************************
** Form generated from reading UI file 'gotodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOTODIALOG_H
#define UI_GOTODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_GoToDialog
{
public:
    QPushButton *gotoButton;
    QSpinBox *gotoSpin;

    void setupUi(QDialog *GoToDialog)
    {
        if (GoToDialog->objectName().isEmpty())
            GoToDialog->setObjectName(QStringLiteral("GoToDialog"));
        GoToDialog->resize(240, 38);
        gotoButton = new QPushButton(GoToDialog);
        gotoButton->setObjectName(QStringLiteral("gotoButton"));
        gotoButton->setGeometry(QRect(136, 8, 96, 24));
        gotoButton->setAutoDefault(true);
        gotoButton->setFlat(false);
        gotoSpin = new QSpinBox(GoToDialog);
        gotoSpin->setObjectName(QStringLiteral("gotoSpin"));
        gotoSpin->setGeometry(QRect(8, 8, 128, 24));
        gotoSpin->setValue(1);

        retranslateUi(GoToDialog);

        QMetaObject::connectSlotsByName(GoToDialog);
    } // setupUi

    void retranslateUi(QDialog *GoToDialog)
    {
        GoToDialog->setWindowTitle(QApplication::translate("GoToDialog", "Go to...", Q_NULLPTR));
        gotoButton->setText(QApplication::translate("GoToDialog", "Go", Q_NULLPTR));
        gotoSpin->setSuffix(QString());
    } // retranslateUi

};

namespace Ui {
    class GoToDialog: public Ui_GoToDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOTODIALOG_H
