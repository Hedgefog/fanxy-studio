#include "createprojectdialog.h"
#include "ui_createprojectdialog.h"

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

CreateProjectDialog::CreateProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateProjectDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
}

CreateProjectDialog::~CreateProjectDialog()
{
    delete ui;
}

void CreateProjectDialog::setProject(ProjectInterface *project)
{
    m_project = project;
}

void CreateProjectDialog::on_workspaceBrowserButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory
    (
        this,
        tr("Select directory"),
        QString(),
        QFileDialog::ShowDirsOnly
     );

    ui->workspaceEdit->setText(path);
}

void CreateProjectDialog::on_acceptButton_clicked()
{
    if(!m_project)
    {
        this->close();
        return;
    }

    m_name = ui->nameEdit->text();
    if(m_name.isEmpty())
        return;

    m_workspace = ui->workspaceEdit->text();
    if(m_workspace.isEmpty())
        return;

    QString ext = m_project->extension();
    QString projectSpace =  m_workspace + "/" + m_name;
    QString path = projectSpace + "/" + m_name + "." + ext;

    if(QFile(path).exists())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("A project with that name already exists");
        msgBox.exec();
        return;
    }

    QDir(m_workspace).mkpath(m_name);

    foreach(const QString &type, m_project->fileTypeList())
    {
        QString dir = m_project->fileTypeDir(type);

        if(!dir.isEmpty())
        {
            if(!QDir(projectSpace).exists(dir))
                QDir(projectSpace).mkdir(dir);
        }
    }

    emit accepted(m_project, path);

    this->close();
}
