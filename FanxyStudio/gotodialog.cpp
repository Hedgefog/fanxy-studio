#include "gotodialog.h"
#include "ui_gotodialog.h"

#include <QTextBlock>

GoToDialog::GoToDialog(QPlainTextEdit *target) :
    ui(new Ui::GoToDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
    m_target = target;
}

GoToDialog::~GoToDialog()
{
    delete ui;
}

void GoToDialog::show()
{
    if(!m_target)
        return;

    int line = m_target->textCursor().blockNumber() + 1;
    ui->gotoSpin->setValue(line);
    ui->gotoSpin->setFocus();

    int maximum = 1;
    int i = m_target->document()->blockCount();
    while(i > 0)
    {
        i /= 10;
        maximum *= 10;
    }

    ui->gotoSpin->setMaximum(maximum*10);

    QDialog::show();
}

void GoToDialog::on_gotoButton_clicked()
{
    if(!m_target)
        return;

    int line = ui->gotoSpin->value();

    if(line < 0 || line > m_target->document()->blockCount())
        return;

    QTextCursor textCursor(m_target->document()->findBlockByLineNumber(line - 1));
    m_target->setTextCursor(textCursor);

    this->close();
}

void GoToDialog::on_gotoSpin_valueChanged(int value)
{
    if(!m_target)
        return;

    int linesCount = m_target->document()->blockCount();

    if(value > linesCount)
        ui->gotoSpin->setValue(linesCount);
}
