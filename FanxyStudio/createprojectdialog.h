#ifndef CREATEPROJECTDIALOG_H
#define CREATEPROJECTDIALOG_H

#include <QDialog>

#include "projectinterface.h"

namespace Ui {
class CreateProjectDialog;
}

class CreateProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateProjectDialog(QWidget *parent = 0);
    ~CreateProjectDialog();

    void                    setProject(ProjectInterface *project);

protected:
    QString                 m_workspace;
    QString                 m_name;
    ProjectInterface        *m_project;

private slots:
    void                    on_workspaceBrowserButton_clicked();
    void                    on_acceptButton_clicked();

signals:
    void                    accepted(ProjectInterface *project, QString path);

private:
    Ui::CreateProjectDialog *ui;
};

#endif // CREATEPROJECTDIALOG_H
