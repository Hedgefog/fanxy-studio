#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

#include <QPlainTextEdit>

namespace Ui {
class GoToDialog;
}

class GoToDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoToDialog(QPlainTextEdit *target);
    ~GoToDialog();

protected:
    QPlainTextEdit       *m_target;

public slots:
    void            show();

protected slots:
    void            on_gotoButton_clicked();
    void            on_gotoSpin_valueChanged(int value);

private:
    Ui::GoToDialog  *ui;
};

#endif // GOTODIALOG_H
