#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QCompleter>
#include <QTextDocument>

#include <QPlainTextEdit>

#include "syntaxtemplateloader.h"

#include "syntaxhighlighter.h"
#include "skinloader.h"
#include "completeritemmodel.h"
#include "linenumberwidget.h"

class Bracket
{
public:
    Bracket(char _start, char _end)
    {
        start = _start;
        end = _end;
    }

    char start;
    char end;
};

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEdit(QWidget *parent = 0);

    SyntaxHighlighter                               *highlighter();
    QCompleter                                      *completer();

    void                                            setDocument(QTextDocument *document);
    void                                            setSyntaxTemplate(QTextDocument *document, QString templateName);
//    void                                            setHighlightList(QTextDocument *document, QStringList highlightList);
    void                                            setCompleterItems(QTextDocument *document, SyntaxTemplateLoader::CompletionDataList completionDataList);

protected slots:
    void                                            updateFont();
    void                                            updateDocumentFont();
    void                                            updateCursor();
    void                                            updateScroll();

    void                                            updateHighlighter();
    void                                            updateCompleter();

    void                                            updateExtraSelection();
    void                                            updateExtraSelectionLine();
    void                                            updateExtraSelectionText();
    void                                            updateExtraSelectionBrackets();

    void                                            on_completer_activated(QString text);
    void                                            on_skinLoader_loaded();
    void                                            on_lineNumberWidget_widthChanged(int width);

protected:
    SyntaxTemplateLoader *m_syntaxTemplateLoader;
    LineNumberWidget                                *m_lineNumberWidget;
    CompleterItemModel                              *m_completerItemModel;
    SettingLoader                                   *m_settingLoader;
    SkinLoader                                      *m_skinLoader;
    SyntaxHighlighter                               *m_highlighter;
    QCompleter                                      *m_completer;
    QList<Bracket>                                  m_bracketList;
    QList<QTextEdit::ExtraSelection>                m_selectionList;
    QMap<QTextDocument*, SyntaxTemplateLoader::CompletionDataList> m_completerItemsMap;
    QMap<QTextDocument*, QString>                   m_syntaxTemplateMap;
    QMap<QTextDocument*, QStringList>               m_highlightListMap;
    QMap<QTextDocument*, int>                       m_currentBlockMap;
    QMap<QTextDocument*, int>                       m_currentScrollMap;

    bool                                            isBracket(QChar ch);

    void                                            doAutoIndent(QKeyEvent *event);
    void                                            doAutoInsert(QKeyEvent *event);

    void                                            putTab();
    void                                            deleteTab();

    void                                            keyPressEvent(QKeyEvent *event);
    void                                            wheelEvent(QWheelEvent *event);
    void                                            resizeEvent(QResizeEvent *event);

signals:
    void                                            documentChanged(QTextDocument *document);

public slots:
};

#endif // CODEEDIT_H
