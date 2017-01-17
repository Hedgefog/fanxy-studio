#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

#include <QPlainTextEdit>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QPlainTextEdit *target);
    ~FindDialog();

protected:
    QPlainTextEdit       *m_target;

public slots:
    void            show();
    void            find();

protected slots:
    void            on_findButton_clicked();
    void            on_replaceButton_clicked();
    void            on_replaceAllButton_clicked();
    void            on_cancelButton_clicked();

private:
    Ui::FindDialog  *ui;
};

#endif // FINDDIALOG_H
