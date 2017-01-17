#include "syntaxhighlighter.h"

#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "skinloader.h"

#include <QDebug>
#include "logger.h"

#define DIR_HIGHLIGHT_TEMPLATES "highlight"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    m_syntaxTemplateLoader = SyntaxTemplateLoader::instance();
    m_settingLoader = SettingLoader::instance();
    m_skinLoader = SkinLoader::instance();

    connect(m_skinLoader, SIGNAL(loaded()), this, SLOT(on_skinLoader_loaded()));
}

//void SyntaxHighlighter::setHighlightList(QStringList highlightList)
//{
//    m_highlightList = highlightList;
//}

void SyntaxHighlighter::setSyntaxTemplate(QString templateName)
{
    m_syntaxTemplate = templateName;
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    if(m_syntaxTemplate.isEmpty())
        return;

    highlightSingleLine(text);
    highlightMultiLine(text);
}

void SyntaxHighlighter::highlightSingleLine(const QString &text)
{
    SyntaxTemplateLoader::HighlightRuleList highlightList;
    highlightList = m_syntaxTemplateLoader->highlightRuleList(m_syntaxTemplate);

    QList<Border> borderList;
    for(int i = 0; i < highlightList.count(); i++)
    {
        QRegExp regExp(highlightList[i].pattern);
        QString formatKey       = highlightList[i].formatKey;
        QColor color  = m_skinLoader->syntaxFormat(formatKey).foreground().color();

        if(!color.isValid() || color == QTextCharFormat().foreground().color())
            continue;

        //bool isChar = (formatKey == "Char");
        bool isComment = (formatKey == "Comment");
        bool isNumber = (formatKey == "Number");
        int index = regExp.indexIn(text);

        while(index >= 0)
        {
            int length = regExp.matchedLength();

            bool isOverlay = false;
            foreach(const Border &border, borderList)
            {
                int endPos = index + length;

                if(index >= border.pos1 && index <= border.pos2)
                {
                    if(border.ownerName != "Function" || isNumber)
                        isOverlay = true;

                    break;
                }
                else if(endPos >= border.pos1 && endPos <= border.pos2)
                {
                    if(border.ownerName != "Function" || isNumber)
                        isOverlay = true;

                    break;
                }
            }

            if(!isOverlay)
            {
                setFormat(index, length, color);

                Border border;
                border.pos1 = index;
                border.pos2 = index + length - 1;
                border.ownerName = formatKey;

                borderList.append(border);

                if(isComment)
                    break;
            }

            index = regExp.indexIn(text, index + length);
        }
    }


    setCurrentBlockState(0);
    /*for(int i = 0; i < m_highlightList.count(); i++)
    {
        QRegExp regExp("\\b" + m_highlightList[i] + "\\b");
        QTextCharFormat format  = m_skinLoader->syntaxFormat("Stock");

        format.setFont(this->document()->defaultFont());

        int index = regExp.indexIn(text);
        while(index >= 0)
        {
            int length = regExp.matchedLength();
            setFormat(index, length, format);
            index = regExp.indexIn(text, index + length);
        }
    }*/
}

void SyntaxHighlighter::highlightMultiLine(const QString &text)
{
    SyntaxTemplateLoader::MultihighlightRuleList multihighlightList;
    multihighlightList = m_syntaxTemplateLoader->multihighlightRuleList(m_syntaxTemplate);

    for(int i = 0; i < multihighlightList.count(); i++)
    {
        QRegExp regExpStart(multihighlightList[i].startPattern);
        QRegExp regExpEnd(multihighlightList[i].endPattern);
        QString formatKey       = multihighlightList[i].formatKey;

        QColor color  = m_skinLoader->syntaxFormat(formatKey).foreground().color();

        if(!color.isValid())
            continue;

        int startIdx = 0;
        int endIdx = 0;
        int commentLength = 0;

        if(previousBlockState() != 1)
        {
            startIdx = regExpStart.indexIn(text);

            SyntaxTemplateLoader::HighlightRuleList highlightList;
            highlightList = m_syntaxTemplateLoader->highlightRuleList(m_syntaxTemplate);
            for(int j = 0; j < highlightList.count(); j++)
            {
                if(formatKey != highlightList[j].formatKey)
                    continue;

                QRegExp regExp(highlightList[i].pattern);
                QString buffer = text.left(startIdx + regExpStart.matchedLength());
                int _index = regExp.indexIn(buffer);

                if(_index >= 0)
                    return;
            }
        }

        while(startIdx >= 0)
        {
            endIdx = regExpEnd.indexIn(text, startIdx);

            if(endIdx == -1)
            {
                setCurrentBlockState(1);
                commentLength = text.length() - startIdx;
            }
            else
            {
                commentLength = endIdx - startIdx + regExpEnd.matchedLength();
            }

            setFormat(startIdx, commentLength, color);
            startIdx = regExpStart.indexIn(text, startIdx + commentLength);
        }
    }
}

void SyntaxHighlighter::on_skinLoader_loaded()
{
    rehighlight();
}
