#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QCompleter>

#include <QVector>
#include <QHBoxLayout>

#include <QTextDocument>

#include "linenumberwidget.h"
#include "syntaxhighlighter.h"

#include "skinloader.h"
#include "codeedit.h"

class CodeEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    CodeEdit *editor();
    LineNumberWidget *lineNumberWidget();

private slots:
    void on_editor_documentChanged(QTextDocument *document);

protected:
    QHBoxLayout *m_layout;
    CodeEdit *m_editor;
    LineNumberWidget *m_lineNumberWidget;

signals:

public slots:
};

#endif // CODEEDITOR_H
