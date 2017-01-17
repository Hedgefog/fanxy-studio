#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QPlainTextEdit *target) :
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    m_target = target;
    setFixedSize(size());
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::show()
{
    if(!m_target)
        return;

    this->hide();

    QString text = m_target->textCursor().selectedText();

    if(!text.isEmpty())
        ui->findEdit->setText(text);

    ui->findEdit->setFocus();

    QDialog::show();
}

void FindDialog::on_findButton_clicked()
{
    find();
}

void FindDialog::find()
{
    if(!m_target)
        return;

    if(!m_target->isVisible())
        return;

    QString request = ui->findEdit->text();

    if(request.isEmpty())
        return;

    QTextDocument::FindFlags flags;

    if(ui->directionUp->isChecked())
        flags |= QTextDocument::FindBackward;

    if(ui->matchcaseCheck->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    if(ui->matchwordCheck->isChecked())
        flags |= QTextDocument::FindWholeWords;

    bool result = m_target->find(request, flags);

    if(!result)
    {
        QTextCursor textCursor = m_target->textCursor();

        if(ui->directionUp->isChecked())
            textCursor.setPosition(QTextCursor::End);
        else
            textCursor.setPosition(QTextCursor::Start);

        m_target->setTextCursor(textCursor);

        m_target->find(request, flags);
    }
}

void FindDialog::on_replaceButton_clicked()
{
    if(m_target->textCursor().selectedText() != ui->findEdit->text())
        on_findButton_clicked();

    if(m_target->textCursor().selectedText() == ui->findEdit->text())
        m_target->textCursor().insertText(ui->replaceEdit->text());
}

void FindDialog::on_replaceAllButton_clicked()
{
    QString text = m_target->toPlainText();

    if(text.isEmpty())
        return;

    QString value = ui->findEdit->text();

    if(value.isEmpty())
        return;

    if(!text.contains(value))
        return;

    QString newValue = ui->replaceEdit->text();

    if(ui->matchcaseCheck->isChecked())
        text.replace(value, newValue, Qt::CaseSensitive);
    else
        text.replace(value, newValue, Qt::CaseInsensitive);

    m_target->selectAll();
    m_target->textCursor().insertText(text);
}

void FindDialog::on_cancelButton_clicked()
{
    this->close();
}
