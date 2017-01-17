/********************************************************************************
** Form generated from reading UI file 'settingwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGWINDOW_H
#define UI_SETTINGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingWindow
{
public:
    QGroupBox *editorGroupBox;
    QComboBox *defaultHighlightSelectBox;
    QLabel *defaultHighlightLabel;
    QLabel *fontLabel;
    QFontComboBox *fontSelectBox;
    QLabel *fontSizeLabel;
    QSpinBox *fontSizeSpinBox;
    QCheckBox *autoIndentCheckBox;
    QCheckBox *autoInsertCheckBox;
    QGroupBox *projectPluginGroupBox;
    QComboBox *projectPluginSelectBox;
    QWidget *projectPluginSetupWidget;
    QVBoxLayout *projectPluginLayout;
    QGroupBox *generalGroupBox;
    QComboBox *skinSelectBox;
    QLabel *skinLabel;
    QPushButton *okButton;
    QPushButton *applyButton;

    void setupUi(QDialog *SettingWindow)
    {
        if (SettingWindow->objectName().isEmpty())
            SettingWindow->setObjectName(QStringLiteral("SettingWindow"));
        SettingWindow->resize(596, 400);
        editorGroupBox = new QGroupBox(SettingWindow);
        editorGroupBox->setObjectName(QStringLiteral("editorGroupBox"));
        editorGroupBox->setGeometry(QRect(8, 152, 328, 209));
        editorGroupBox->setStyleSheet(QStringLiteral(""));
        defaultHighlightSelectBox = new QComboBox(editorGroupBox);
        defaultHighlightSelectBox->setObjectName(QStringLiteral("defaultHighlightSelectBox"));
        defaultHighlightSelectBox->setGeometry(QRect(104, 60, 216, 24));
        defaultHighlightLabel = new QLabel(editorGroupBox);
        defaultHighlightLabel->setObjectName(QStringLiteral("defaultHighlightLabel"));
        defaultHighlightLabel->setGeometry(QRect(8, 60, 96, 24));
        fontLabel = new QLabel(editorGroupBox);
        fontLabel->setObjectName(QStringLiteral("fontLabel"));
        fontLabel->setGeometry(QRect(8, 20, 48, 24));
        fontSelectBox = new QFontComboBox(editorGroupBox);
        fontSelectBox->setObjectName(QStringLiteral("fontSelectBox"));
        fontSelectBox->setGeometry(QRect(56, 20, 160, 24));
        fontSizeLabel = new QLabel(editorGroupBox);
        fontSizeLabel->setObjectName(QStringLiteral("fontSizeLabel"));
        fontSizeLabel->setGeometry(QRect(224, 20, 48, 24));
        fontSizeSpinBox = new QSpinBox(editorGroupBox);
        fontSizeSpinBox->setObjectName(QStringLiteral("fontSizeSpinBox"));
        fontSizeSpinBox->setGeometry(QRect(272, 20, 48, 24));
        fontSizeSpinBox->setWrapping(false);
        fontSizeSpinBox->setFrame(true);
        fontSizeSpinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        fontSizeSpinBox->setAccelerated(false);
        fontSizeSpinBox->setKeyboardTracking(true);
        fontSizeSpinBox->setProperty("showGroupSeparator", QVariant(false));
        fontSizeSpinBox->setMaximum(64);
        fontSizeSpinBox->setValue(0);
        autoIndentCheckBox = new QCheckBox(editorGroupBox);
        autoIndentCheckBox->setObjectName(QStringLiteral("autoIndentCheckBox"));
        autoIndentCheckBox->setGeometry(QRect(8, 104, 96, 24));
        autoInsertCheckBox = new QCheckBox(editorGroupBox);
        autoInsertCheckBox->setObjectName(QStringLiteral("autoInsertCheckBox"));
        autoInsertCheckBox->setGeometry(QRect(8, 136, 96, 24));
        projectPluginGroupBox = new QGroupBox(SettingWindow);
        projectPluginGroupBox->setObjectName(QStringLiteral("projectPluginGroupBox"));
        projectPluginGroupBox->setGeometry(QRect(344, 8, 244, 352));
        projectPluginSelectBox = new QComboBox(projectPluginGroupBox);
        projectPluginSelectBox->setObjectName(QStringLiteral("projectPluginSelectBox"));
        projectPluginSelectBox->setGeometry(QRect(0, 24, 244, 24));
        projectPluginSetupWidget = new QWidget(projectPluginGroupBox);
        projectPluginSetupWidget->setObjectName(QStringLiteral("projectPluginSetupWidget"));
        projectPluginSetupWidget->setGeometry(QRect(0, 48, 244, 304));
        projectPluginLayout = new QVBoxLayout(projectPluginSetupWidget);
        projectPluginLayout->setSpacing(0);
        projectPluginLayout->setObjectName(QStringLiteral("projectPluginLayout"));
        projectPluginLayout->setContentsMargins(0, 0, 0, 0);
        generalGroupBox = new QGroupBox(SettingWindow);
        generalGroupBox->setObjectName(QStringLiteral("generalGroupBox"));
        generalGroupBox->setGeometry(QRect(8, 8, 328, 136));
        skinSelectBox = new QComboBox(generalGroupBox);
        skinSelectBox->setObjectName(QStringLiteral("skinSelectBox"));
        skinSelectBox->setGeometry(QRect(104, 24, 220, 24));
        skinLabel = new QLabel(generalGroupBox);
        skinLabel->setObjectName(QStringLiteral("skinLabel"));
        skinLabel->setGeometry(QRect(8, 24, 96, 24));
        okButton = new QPushButton(SettingWindow);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(460, 368, 128, 24));
        applyButton = new QPushButton(SettingWindow);
        applyButton->setObjectName(QStringLiteral("applyButton"));
        applyButton->setGeometry(QRect(324, 368, 128, 24));

        retranslateUi(SettingWindow);

        QMetaObject::connectSlotsByName(SettingWindow);
    } // setupUi

    void retranslateUi(QDialog *SettingWindow)
    {
        SettingWindow->setWindowTitle(QApplication::translate("SettingWindow", "Options", Q_NULLPTR));
        editorGroupBox->setTitle(QApplication::translate("SettingWindow", "Editor", Q_NULLPTR));
        defaultHighlightLabel->setText(QApplication::translate("SettingWindow", "Default highlight:", Q_NULLPTR));
        fontLabel->setText(QApplication::translate("SettingWindow", "Font:", Q_NULLPTR));
        fontSelectBox->setCurrentText(QString());
        fontSizeLabel->setText(QApplication::translate("SettingWindow", "Size:", Q_NULLPTR));
        autoIndentCheckBox->setText(QApplication::translate("SettingWindow", "Auto Indent", Q_NULLPTR));
        autoInsertCheckBox->setText(QApplication::translate("SettingWindow", "Auto Insert", Q_NULLPTR));
        projectPluginGroupBox->setTitle(QApplication::translate("SettingWindow", "Project Plugin", Q_NULLPTR));
        generalGroupBox->setTitle(QApplication::translate("SettingWindow", "Genreal", Q_NULLPTR));
        skinLabel->setText(QApplication::translate("SettingWindow", "Skin", Q_NULLPTR));
        okButton->setText(QApplication::translate("SettingWindow", "Ok", Q_NULLPTR));
        applyButton->setText(QApplication::translate("SettingWindow", "Apply", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SettingWindow: public Ui_SettingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGWINDOW_H
