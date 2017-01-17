#include "codeeditor.h"

#include <QAbstractTextDocumentLayout>
#include <QDebug>
#include "completeritemmodel.h"

CodeEditor::CodeEditor(QWidget *parent) : QWidget(parent)
{
    m_editor = new CodeEdit;

    m_lineNumberWidget = new LineNumberWidget;
    m_lineNumberWidget->setEditor(m_editor);

    m_layout = new QHBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->insertWidget(0, m_lineNumberWidget);
    m_layout->insertWidget(1, m_editor);

    this->setLayout(m_layout);

    connect(m_editor, SIGNAL(documentChanged(QTextDocument*)), this, SLOT(on_editor_documentChanged(QTextDocument*)));
}

CodeEdit *CodeEditor::editor()
{
    return m_editor;
}

LineNumberWidget *CodeEditor::lineNumberWidget()
{
    return m_lineNumberWidget;
}

void CodeEditor::on_editor_documentChanged(QTextDocument* document)
{
    m_lineNumberWidget->update();
}
