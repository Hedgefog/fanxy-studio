#include "codeedit.h"

#include <QAbstractTextDocumentLayout>
#include <QAbstractItemView>
#include <QScrollBar>

#include <QDebug>

#include "completeritemmodel.h"

#define TAB_INDENT_SIZE 4

CodeEdit::CodeEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    m_syntaxTemplateLoader = SyntaxTemplateLoader::instance();
    m_settingLoader = SettingLoader::instance();
    m_skinLoader = SkinLoader::instance();
    m_highlighter = new SyntaxHighlighter;
    m_completerItemModel = new CompleterItemModel;

    m_lineNumberWidget = new LineNumberWidget(this);
    m_lineNumberWidget->setEditor(this);

    m_bracketList.append(Bracket('{', '}'));
    m_bracketList.append(Bracket('(', ')'));
    m_bracketList.append(Bracket('[', ']'));
    m_bracketList.append(Bracket('\"', '\"'));
    m_bracketList.append(Bracket('\'', '\''));

    m_completer = new QCompleter;
    m_completer->setModel(m_completerItemModel);
    m_completer->setCompletionRole(CompleterItemModel::NameRole);
    m_completer->setCaseSensitivity(Qt::CaseSensitive);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setWidget(this);

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateExtraSelection()));
    connect(m_settingLoader, SIGNAL(loaded()), this, SLOT(updateFont()));
    connect(m_skinLoader, SIGNAL(loaded()), this, SLOT(on_skinLoader_loaded()));
    connect(m_completer, SIGNAL(activated(QString)), this, SLOT(on_completer_activated(QString)));
    connect(m_lineNumberWidget, SIGNAL(widthChanged(int)), this, SLOT(on_lineNumberWidget_widthChanged(int)));

    //setAcceptRichText(false);
    setLineWrapMode(QPlainTextEdit::NoWrap);
    updateFont();
}

void CodeEdit::setDocument(QTextDocument *document)
{
    QTextDocument *currentDocument = this->document();

    if(currentDocument)
    {
          m_currentBlockMap[currentDocument] = this->textCursor().blockNumber();
          m_currentScrollMap[currentDocument] = this->verticalScrollBar()->value();
    }

    if(!document)
        return;

    QPlainTextEdit::setDocument(document);

    updateDocumentFont();
    updateHighlighter();
    updateCompleter();
    updateCursor();
    updateScroll();

    emit documentChanged(document);
}

void CodeEdit::setSyntaxTemplate(QTextDocument *document, QString templateName)
{
    if(!document)
        return;

    m_syntaxTemplateMap[document] = templateName;
    QTextDocument *_document = this->document();

    if(document == _document)
    {
        updateHighlighter();
        updateCompleter();
    }
}

//void CodeEdit::setHighlightList(QTextDocument *document, QStringList highlightList)
//{
//    if(!document)
//        return;

//    m_highlightListMap[document] = highlightList;

//    QTextDocument *_document = this->document();

//    if(document == _document)
//        updateHighlighter();
//}

void CodeEdit::setCompleterItems(QTextDocument *document, SyntaxTemplateLoader::CompletionDataList completionDataList)
{
    if(!document)
        return;

    m_completerItemsMap[document] = completionDataList;
    QTextDocument *_document = this->document();

    if(document == _document)
        updateCompleter();
}

void CodeEdit::putTab()
{
    QString insertion = "\t";

    QTextCursor cursor = textCursor();
    if (cursor.selectionEnd() - cursor.selectionStart() <= 0)
    {
        insertPlainText(insertion);
    }
    else
    {
        QTextBlock firstBlock = document()->findBlock(cursor.selectionStart());
        QTextBlock lastBlock = document()->findBlock(cursor.selectionEnd() - 1);

        bool isSingleBlockSelection = (firstBlock == lastBlock);
        bool doSingleBlockTab = false;

        if(isSingleBlockSelection)
            doSingleBlockTab = (cursor.selectedText().length() == firstBlock.text().length());

        cursor.setPosition(firstBlock.position());
        cursor.beginEditBlock();

        if(!isSingleBlockSelection || doSingleBlockTab)
        {
            do
            {
                cursor.insertText(insertion);
            }
            while(cursor.movePosition(QTextCursor::NextBlock) && cursor.position() <= lastBlock.position());
        }
        else
        {
            insertPlainText(insertion);
        }

        cursor.endEditBlock();
    }
}

void CodeEdit::deleteTab()
{
    QTextCursor cursor = textCursor();
    if (cursor.selectionEnd() - cursor.selectionStart() <= 0)
    {
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
        QString selected = cursor.selectedText();
        if (selected.startsWith('\t'))
            cursor.deletePreviousChar();
    }
    else
    {
        QTextBlock firstBlock = document()->findBlock(cursor.selectionStart());
        QTextBlock lastBlock = document()->findBlock(cursor.selectionEnd() - 1);

        cursor.setPosition(firstBlock.position());
        cursor.beginEditBlock();

        do
        {
            if (cursor.block().text().startsWith('\t'))
            {
                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
                cursor.removeSelectedText();
            }
            else if(cursor.block().text().startsWith(' '))
            {
                int indendSize = 0;

                for(int i = 0; i < TAB_INDENT_SIZE; i++)
                {
                    if(cursor.block().text()[i] == ' ')
                        indendSize++;
                    else
                        break;
                }

                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, indendSize);
                cursor.removeSelectedText();
            }
        } while (cursor.movePosition(QTextCursor::NextBlock) && cursor.position() <= lastBlock.position());
        cursor.endEditBlock();
    }
}

void CodeEdit::updateFont()
{
    setFont(m_settingLoader->font());
    updateDocumentFont();
}

void CodeEdit::updateDocumentFont()
{
    QTextDocument *document = this->document();

    if(!document)
        return;

    document->setDefaultFont(this->font());
    m_lineNumberWidget->update();
    setTabStopWidth(TAB_INDENT_SIZE * QFontMetrics(this->font()).width(' '));
}

void CodeEdit::updateCursor()
{
    QTextDocument *document = this->document();

    if(!document)
        return;

    if(!m_currentBlockMap.contains(document))
        return;

    int blockNumber = m_currentBlockMap[document];

    if(blockNumber < 0)
        return;

    if(blockNumber >= document->blockCount())
        blockNumber = document->lastBlock().position();

    QTextCursor textCursor(document->findBlockByLineNumber(blockNumber));
    setTextCursor(textCursor);
}

void CodeEdit::updateScroll()
{
    QTextDocument *document = this->document();

    if(!document)
        return;

    if(!m_currentScrollMap.contains(document))
        return;

    this->verticalScrollBar()->setValue(m_currentScrollMap[document]);
}

void CodeEdit::updateHighlighter()
{
    QTextDocument *document = this->document();

    if(!document)
        return;

    if(m_syntaxTemplateMap.contains(document))
        m_highlighter->setSyntaxTemplate(m_syntaxTemplateMap[document]);
    else
        m_highlighter->setSyntaxTemplate(m_settingLoader->defHighlightTemplate());

//    if(m_highlightListMap.contains(document))
//        m_highlighter->setHighlightList(m_highlightListMap[document]);

    m_highlighter->setDocument(document);
}

void CodeEdit::updateCompleter()
{
    QTextDocument *document = this->document();

    if(!document)
        return;

    CompleterItemModel *model = (CompleterItemModel*)m_completer->model();

    if(model)
        model->clear();

    QString templateName;

    if(m_syntaxTemplateMap.contains(document))
        templateName = m_syntaxTemplateMap[document];
    else
        templateName = m_settingLoader->defHighlightTemplate();

    if(templateName.isEmpty())
        return;

    SyntaxTemplateLoader::CompletionDataList completerItems;

    completerItems << m_syntaxTemplateLoader->completionDataList(templateName);

    if(m_completerItemsMap.contains(document))
        completerItems << m_completerItemsMap[document];

    model->addItems(completerItems);
}

void CodeEdit::updateExtraSelection()
{
    m_selectionList.clear();
    updateExtraSelectionText();
    updateExtraSelectionLine();
    updateExtraSelectionBrackets();
    this->setExtraSelections(m_selectionList);
}

void CodeEdit::updateExtraSelectionText()
{
    QTextCharFormat format = m_skinLoader->editorFormat("TEXTHIGHLIGHT");

    QRegExp exp = QRegExp(this->textCursor().selectedText());
    QString text = this->document()->toPlainText();

    int pos = 0;
    int len = 0;

    while((pos = text.indexOf(exp, pos + len)) != -1)
    {
        len = exp.matchedLength();

        if(len <= 0)
            return;

        QTextEdit::ExtraSelection selection;
        selection.cursor = QTextCursor(this->document()->docHandle(), pos);
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, len);
        selection.format = format;
        m_selectionList.append(selection);
    }
}

void CodeEdit::updateExtraSelectionLine()
{
    QTextCharFormat format = m_skinLoader->editorFormat("LINEHIGHLIGHT");

    QTextEdit::ExtraSelection selection;
    selection.format = format;
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = this->textCursor();
    m_selectionList.append(selection);
}

void CodeEdit::updateExtraSelectionBrackets()
{
    int posStart = textCursor().position();

    QChar bracketStart = document()->characterAt(posStart);
    QChar bracketEnd;

    if(!isBracket(bracketStart))
    {
        posStart--;
        bracketStart = document()->characterAt(posStart);

        if(!isBracket(bracketStart))
            return;
    }

    int direction = 0;
    for(int i = 0; i < m_bracketList.count(); i++)
    {
        if(bracketStart == m_bracketList.at(i).start)
        {
            bracketEnd = m_bracketList.at(i).end;
            direction = 1;
            break;
        }
        else if(bracketStart == m_bracketList.at(i).end)
        {
            bracketEnd = m_bracketList.at(i).start;
            direction = -1;
            break;
        }
    }

    int posEnd = -1;
    int count = 0;
    int stop = (direction == 1) ? (document()->characterCount()) : (-1);

    for(int i = posStart; i != stop; i += direction)
    {
        QChar ch = document()->characterAt(i);

        if(ch == bracketStart)
        {
            count++;
        }
        else if(ch == bracketEnd)
        {
            count--;

            if(count == 0)
            {
                posEnd = i;
                break;
            }
        }
    }

    if(posEnd == -1)
        return;

    //Setup highlight format
    QTextCharFormat format = m_skinLoader->editorFormat("BRACEHIGHLIGHT");
    format.setFont(document()->defaultFont());
    QTextEdit::ExtraSelection selection;
    selection.format = format;

    //Setup highlight cursor
    selection.cursor = textCursor();

    //Highlight first bracket
    selection.cursor.setPosition(posStart);
    selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    m_selectionList.append(selection);

    //Highlight second bracket
    selection.cursor.setPosition(posEnd);
    selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    m_selectionList.append(selection);
}

void CodeEdit::doAutoIndent(QKeyEvent *event)
{
    if(!m_settingLoader->autoIndent())
    {
        QPlainTextEdit::keyPressEvent(event);
        return;
    }

    QString indent;

    QString blockText = textCursor().block().text();

    int spaceCount = 0;

    for(int i = 0; i < blockText.length(); i++)
    {
        if(blockText[i] == '\t')
        {
            indent += blockText[i];
        }
        else if(blockText[i] == ' ')
        {
            spaceCount++;

            if(spaceCount == TAB_INDENT_SIZE)
            {
                spaceCount = 0;
                indent += '\t';
            }
        }
        else
        {
            break;
        }
    }

    int cursorPosition = textCursor().position();

    int indentCount = 0;
    for(int i = cursorPosition; i < cursorPosition + textCursor().block().text().size(); i++)
    {
        char ch = document()->characterAt(i).toLatin1();

        if(ch == '\t' || ch == ' ')
            indentCount++;
        else
            break;
    }

    QTextCursor cursor = textCursor();
    cursor.setPosition(cursorPosition + indentCount, QTextCursor::KeepAnchor);
    setTextCursor(cursor);
    textCursor().removeSelectedText();

    char cursorChar = document()->characterAt(cursorPosition).toLatin1();
    char previousChar = 0;

    if(cursorPosition > 0)
        previousChar = document()->characterAt(cursorPosition-1).toLatin1();

    bool cursorCharIsBracket = false;
    bool previousCharIsBracket = false;

    if(previousChar)
    {
        for(int i = 0; i < m_bracketList.count(); i++)
        {
            if(previousChar == m_bracketList.at(i).start)
                previousCharIsBracket = true;

            if(cursorChar == m_bracketList.at(i).end)
                cursorCharIsBracket = true;

            if(previousCharIsBracket && cursorCharIsBracket)
                break;
        }

        if(previousCharIsBracket && cursorCharIsBracket)
        {
            indent += '\t';

            cursorPosition = textCursor().position() + indent.length() + 1;

            if(cursorCharIsBracket)
                indent += '\n' + indent.left(indent.size() - 1);
        }
    }

    QPlainTextEdit::keyPressEvent(event);
    textCursor().insertText(indent);

    if(cursorCharIsBracket && previousCharIsBracket)
    {
        QTextCursor cursor = textCursor();
        cursor.setPosition(cursorPosition);
        setTextCursor(cursor);
    }
}

void CodeEdit::doAutoInsert(QKeyEvent *event)
{
    if(!m_settingLoader->autoInsert())
    {
        QPlainTextEdit::keyPressEvent(event);
        return;
    }

    QString text = event->text();

    char ch = text[0].toLatin1();

    QTextCursor textCursor = this->textCursor();
    QString selectedText = textCursor.selectedText();

    char chNext = document()->characterAt(textCursor.position()).toLatin1();
    char chPrev = document()->characterAt(textCursor.position() - 1).toLatin1();

    for(int i = 0; i < m_bracketList.count(); i++)
    {
        if(ch == m_bracketList.at(i).start)
        {
            text = ch + selectedText + m_bracketList.at(i).end;
            textCursor.insertText(text);
            textCursor.setPosition(textCursor.position() - 1);
            setTextCursor(textCursor);
            event->ignore();
            return;
        }
        else if(ch == m_bracketList.at(i).end)
        {
            if(chNext == ch && chPrev == m_bracketList.at(i).start)
            {
                textCursor.setPosition(textCursor.position() + 1);
                setTextCursor(textCursor);
                event->ignore();
                return;
            }
        }
    }

    QPlainTextEdit::keyPressEvent(event);
}

void CodeEdit::on_completer_activated(QString text)
{
    QTextCursor cursor = this->textCursor();
    int extra = text.length() - m_completer->completionPrefix().length();
    cursor.movePosition(QTextCursor::Left);
    cursor.movePosition(QTextCursor::EndOfWord);
    cursor.insertText(text.right(extra));
    this->setTextCursor(cursor);
}

void CodeEdit::on_skinLoader_loaded()
{
    updateExtraSelection();
}

void CodeEdit::on_lineNumberWidget_widthChanged(int width)
{
    setViewportMargins(width, 0, 0, 0);
}

void CodeEdit::resizeEvent(QResizeEvent *event)
{
    m_lineNumberWidget->setGeometry(0, 0, m_lineNumberWidget->width(), event->size().height());

    QPlainTextEdit::resizeEvent(event);
}

void CodeEdit::keyPressEvent(QKeyEvent *event)
{
    Qt::KeyboardModifiers modifiers = event->modifiers();
    int key = event->key();
    QString text = event->text();

    if(m_completer->popup()->isVisible())
    {
       switch (key)
       {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            event->ignore();
            return;
       }
    }

    bool isShortcut = ((modifiers & Qt::ControlModifier) && key == Qt::Key_E);

    if  (!isShortcut)
    {
        switch(key)
        {
        case Qt::Key_Tab:
            putTab();
            event->accept();
            break;
        case Qt::Key_Backtab:
            deleteTab();
            event->accept();
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            doAutoIndent(event);
            break;
        case Qt::Key_QuoteDbl:
        case Qt::Key_Apostrophe:
        case Qt::Key_ParenLeft:
        case Qt::Key_BraceLeft:
        case Qt::Key_BracketLeft:    
        case Qt::Key_ParenRight:
        case Qt::Key_BraceRight:
        case Qt::Key_BracketRight:
            doAutoInsert(event);
            break;
        default:
            QPlainTextEdit::keyPressEvent(event);
            break;
        }
    }

    QTextCursor cursor = this->textCursor();
    QString blockText = cursor.block().text();
    int endPos = cursor.position() - cursor.block().position();
    int startPos = endPos;

    while(startPos > 0)
    {
        startPos--;

        if(!blockText[startPos].isLetterOrNumber() && blockText[startPos] != '_')
        {
            startPos++;
            break;
        }
    }

    QString prefix = blockText.mid(startPos, endPos - startPos);

    if(text.isEmpty() || prefix.length() < 3)
    {
        m_completer->popup()->hide();
        return;
    }

    if(prefix != m_completer->completionPrefix())
    {
        m_completer->setCompletionPrefix(prefix);
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
    }

    QRect rect = cursorRect();
    rect.setWidth(m_completer->popup()->sizeHintForColumn(0) + m_completer->popup()->verticalScrollBar()->sizeHint().width());
    m_completer->complete(rect);
}

void CodeEdit::wheelEvent(QWheelEvent *event)
{
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if(~modifiers & Qt::ControlModifier)
    {
        QPlainTextEdit::wheelEvent(event);
        return;
    }

    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal)
        this->zoomOut(numSteps);
    else
        this->zoomIn(numSteps);

    m_lineNumberWidget->updateWidth();

    double width = QFontMetricsF(this->font()).width(' ') * TAB_INDENT_SIZE;
    setTabStopWidth(width);

    event->ignore();
}

bool CodeEdit::isBracket(QChar ch)
{
    for(int i = 0; i < m_bracketList.count(); i++)
    {
        if(ch == m_bracketList.at(i).start || ch == m_bracketList.at(i).end)
            return true;
    }

    return false;
}

SyntaxHighlighter *CodeEdit::highlighter()  {   return m_highlighter;   }
QCompleter *CodeEdit::completer()           {   return m_completer;     }
