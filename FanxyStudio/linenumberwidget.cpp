#include "linenumberwidget.h"

#include <QPainter>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QScrollBar>
#include <QLayout>
#include <QPlainTextEdit>

#include "logger.h"
#include <QDebug>

LineNumberWidget::LineNumberWidget(QWidget *parent) : QWidget(parent)
{
    m_editor = NULL;
    //m_currentDocument = NULL;
    m_settingLoader = SettingLoader::instance();
    m_skinLoader = SkinLoader::instance();
}

LineNumberWidget::~LineNumberWidget() {}

void LineNumberWidget::paintEvent(QPaintEvent *event)
{
    if(!m_editor)
        return;

    QTextDocument *document = m_editor->document();
    int lineCount = document->blockCount();
    QTextBlock block = document->firstBlock();

    QColor color = m_skinLoader->editorFormat("LINENUMBERAREA").foreground().color();
    QColor backgroundColor = m_skinLoader->editorFormat("LINENUMBERAREA").background().color();

    QPainter painter(this);

    if(backgroundColor.isValid())
        painter.fillRect(event->rect(), backgroundColor);
    else
        painter.fillRect(event->rect(), Qt::white);

    if(color.isValid())
        painter.setPen(color);
    else
        painter.setPen(Qt::black);

    QFont font = m_editor->font();
    painter.setFont(font);

    int vScrollValue = m_editor->verticalScrollBar()->value();
    int top = -vScrollValue*(int)document->documentLayout()->blockBoundingRect(block).height() + m_editor->contentsMargins().top();

    for (int i = block.blockNumber(); i < lineCount; i++)
    {
        //if(!block.isValid())
            //break;

        if(top >= height())
            break;

        if(block.isVisible() && top >= 0)
            painter.drawText(0, top, width(), QFontMetrics(font).height(), Qt::AlignRight, QString::number(i + 1));

        top += (int)document->documentLayout()->blockBoundingRect(block).height();
        block = block.next();
    }
}

void LineNumberWidget::setEditor(QPlainTextEdit *editor)
{
    if(!editor)
        return;

    m_editor = editor;
    connect(m_editor->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(on_editor_vScrollBar_valueChanged(int)));

    connect(m_editor, SIGNAL(textChanged()), this, SLOT(update()));
    connect(m_editor, SIGNAL(textChanged()), this, SLOT(updateWidth()));

    connect(m_editor, SIGNAL(cursorPositionChanged()), this, SLOT(update()));
    connect(m_editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateWidth()));

    updateWidth();
}

void LineNumberWidget::on_editor_vScrollBar_valueChanged(int)
{
    update();
    updateWidth();
}

void LineNumberWidget::updateWidth()
{
    if(!m_editor)
        return;

    int num = m_editor->document()->blockCount();
    int count = 0;

    while(num > 0)
    {
        num /= 10;
        count++;
    }

    QFont font = m_editor->font();
    int width = 4 + QFontMetrics(font).width('_') * count;

    QRect rect = geometry();
    rect.setWidth(width);

    setGeometry(rect);
    setMinimumWidth(width);

    emit widthChanged(width);
}
