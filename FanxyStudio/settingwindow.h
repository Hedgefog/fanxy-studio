#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>

#include "settingloader.h"
#include "projectpluginloader.h"
#include "skinloader.h"
#include "syntaxtemplateloader.h"

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = 0);
    ~SettingWindow();

protected:
    void                    saveSetting();

private:
    Ui::SettingWindow       *ui;
    SyntaxTemplateLoader    *m_syntaxTemplateLoader;
    ProjectPluginLoader     *m_projectPluginLoader;
    SettingLoader           *m_settingLoader;
    SkinLoader              *m_skinLoader;

protected slots:
    void                    on_okButton_clicked();
    void                    on_projectPluginSelectBox_currentIndexChanged(const QString &arg1);
    void                    on_applyButton_clicked();
    void                    apply();
};

#endif // SETTINGWINDOW_H
