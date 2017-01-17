#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog() :
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::show()
{
    setTitle(m_title);
    ui->textEdit->setText(m_text);
    ui->acceptButton->setText(m_buttonText);

    QDialog::show();
}

void InputDialog::setTitle(QString text)
{
    m_title = text;
}

void InputDialog::setText(QString text)
{
    m_text = text;
}

void InputDialog::setButtonText(QString text)
{
    m_buttonText = text;
}

void InputDialog::on_acceptButton_clicked()
{
    QString text = ui->textEdit->text();

    if(text.isEmpty())
        return;

    emit accepted(text);

    this->close();
}
