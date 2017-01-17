#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog();
    ~InputDialog();

    void            setTitle(QString text = "Input dialog");
    void            setText(QString text = "");
    void            setButtonText(QString text = "Accept");
    void            show();

protected:
    QString         m_title;
    QString         m_text;
    QString         m_buttonText;

private:
    Ui::InputDialog *ui;

protected slots:
    void            on_acceptButton_clicked();


signals:
    void    accepted(QString text);
};

#endif // INPUTDIALOG_H
