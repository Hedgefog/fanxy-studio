#include "settingwindow.h"
#include "ui_settingwindow.h"

#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QDir>

#include "logger.h"

#include "syntaxhighlighter.h"

SettingWindow::SettingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);
    setFixedSize(size());

    m_syntaxTemplateLoader = SyntaxTemplateLoader::instance();
    m_projectPluginLoader = ProjectPluginLoader::instance();
    m_skinLoader = SkinLoader::instance();
    m_settingLoader = SettingLoader::instance();

    ui->defaultHighlightSelectBox->addItems(m_syntaxTemplateLoader->templateList());
    ui->skinSelectBox->addItems(m_skinLoader->skinList());
    ui->projectPluginSelectBox->addItems(m_projectPluginLoader->projectList());

    ui->fontSelectBox->setCurrentText(m_settingLoader->font().family());
    ui->fontSizeSpinBox->setValue(m_settingLoader->font().pointSize());
    ui->skinSelectBox->setCurrentText(m_settingLoader->skin());
    ui->defaultHighlightSelectBox->setCurrentText(m_settingLoader->defHighlightTemplate());
    ui->autoInsertCheckBox->setChecked(m_settingLoader->autoInsert());
    ui->autoIndentCheckBox->setChecked(m_settingLoader->autoIndent());
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::saveSetting()
{
    QJsonObject jObject;
    jObject["Font"]                 = ui->fontSelectBox->currentText();
    jObject["FontSize"]             = ui->fontSizeSpinBox->value();
    jObject["Skin"]                 = ui->skinSelectBox->currentText();
    jObject["DefHighlightTemplate"] = ui->defaultHighlightSelectBox->currentText();
    jObject["AutoInsert"]           = ui->autoInsertCheckBox->isChecked();
    jObject["AutoIndent"]           = ui->autoIndentCheckBox->isChecked();

    QFile file(FILE_CONFIG_SETTINGS);

    if(!file.open(QFile::WriteOnly))
        return;

    file.write(QJsonDocument(jObject).toJson());
    file.close();
}

void SettingWindow::apply()
{
    saveSetting();
    m_settingLoader->load();
    m_skinLoader->load(m_settingLoader->skin());
}

void SettingWindow::on_okButton_clicked()
{
    apply();
    this->close();
}

void SettingWindow::on_applyButton_clicked()
{
    apply();
}

void SettingWindow::on_projectPluginSelectBox_currentIndexChanged(const QString &arg1)
{
    ProjectInterface *instance = m_projectPluginLoader->projectInstance(arg1);

    if(!instance)
        return;

    QLayout *layout = ui->projectPluginSetupWidget->layout();

    while(layout->count() > 0)
    {
        QLayoutItem* item = layout->takeAt(0);

        if(item)
            item->widget()->setParent(0);
    }

    QWidget *widget = instance->setupWidget();

    if(widget)
        layout->addWidget(widget);
}
