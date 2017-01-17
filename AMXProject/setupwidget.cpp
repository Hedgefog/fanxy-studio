#include "setupwidget.h"
#include "ui_setupwidget.h"

#include <QFileDialog>

#include <QDebug>

SetupWidget::SetupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupWidget)
{
    ui->setupUi(this);
}

SetupWidget::~SetupWidget()
{
    delete ui;
}

void SetupWidget::setSetupData(SetupData setupData)
{
    ui->compilerEdit->setText(setupData.compilerPath);
    ui->gamePathEdit->setText(setupData.gamePath);
    ui->gameModEdit->setText(setupData.gameMod);
    ui->executeCmdEdit->setText(setupData.executeCmd);
    ui->outputPathEdit->setText(setupData.outputPath);
}

void SetupWidget::on_applyButton_clicked()
{
    SetupData setupData;
    setupData.compilerPath  = ui->compilerEdit->text();
    setupData.gamePath      = ui->gamePathEdit->text();
    setupData.gameMod       = ui->gameModEdit->text();
    setupData.executeCmd    = ui->executeCmdEdit->text();
    setupData.outputPath    = ui->outputPathEdit->text();

    emit accepted(setupData);
}

void SetupWidget::on_gamePathBrowserButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory
    (
        this,
        tr("Select directory"),
        QString(),
        QFileDialog::ShowDirsOnly
     );

    ui->gamePathEdit->setText(path);
}

void SetupWidget::on_compilerBrowserButton_clicked()
{
    QString path = QFileDialog::getOpenFileName
    (
        this,
        tr("Select directory"),
        QString()
     );

    ui->compilerEdit->setText(path);
}

void SetupWidget::on_outputPathBrowserButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory
    (
        this,
        tr("Select directory"),
        QString(),
        QFileDialog::ShowDirsOnly
     );

    ui->outputPathEdit->setText(path);
}
