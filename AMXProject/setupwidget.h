#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QWidget>

namespace Ui {
class SetupWidget;
}

class SetupWidget : public QWidget
{
    Q_OBJECT
public : struct SetupData
{
    QString compilerPath;
    QString gamePath;
    QString gameMod;
    QString executeCmd;
    QString outputPath;
};

public:
    explicit SetupWidget(QWidget *parent = 0);
    ~SetupWidget();
    void setSetupData(SetupData setupData);

private slots:
    void on_applyButton_clicked();
    void on_gamePathBrowserButton_clicked();
    void on_compilerBrowserButton_clicked();

    void on_outputPathBrowserButton_clicked();

private:
    Ui::SetupWidget *ui;

signals:
    void accepted(SetupWidget::SetupData setupData);
};

#endif // SETUPWIDGET_H
