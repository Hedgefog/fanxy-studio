#ifndef SYNTAXTEMPLATELOADER_H
#define SYNTAXTEMPLATELOADER_H

#include <QTextDocument>
#include <QTextCharFormat>
#include <QRegularExpression>

#include "skinloader.h"

#define DIR_SYNTAX_TEMPLATES   "templates"

class SyntaxTemplateLoader
{
public : struct CompletionData
{
    QString name;
    QString info;
};

public : struct HighlightRule
{
    QString pattern;
    QString formatKey;
};

public : struct MultihighlightRule
{
    QString startPattern;
    QString endPattern;
    QString formatKey;
};

public : typedef QList<CompletionData> CompletionDataList;
public : typedef QList<HighlightRule> HighlightRuleList;
public : typedef QList<MultihighlightRule> MultihighlightRuleList;

public:
    static SyntaxTemplateLoader *instance();
    SyntaxTemplateLoader();

    CompletionDataList      completionDataList(QString templateName);
    HighlightRuleList       highlightRuleList(QString templateName);
    MultihighlightRuleList  multihighlightRuleList(QString templateName);

    QStringList templateList();

protected:
    void                                loadTemplateList();
    void                                loadTemplates();

    static SyntaxTemplateLoader             *m_instance;
    QStringList                             m_templateList;
    QMap<QString, CompletionDataList>       m_completerDataListMap;
    QMap<QString, HighlightRuleList>        m_highlightRuleListMap;
    QMap<QString, MultihighlightRuleList>   m_multihighlightRuleListMap;
};

#endif // SYNTAXTEMPLATELOADER_H
