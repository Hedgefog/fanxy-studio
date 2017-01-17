#include "syntaxtemplateloader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QDir>

#include <QDebug>

SyntaxTemplateLoader *SyntaxTemplateLoader::m_instance = NULL;

SyntaxTemplateLoader *SyntaxTemplateLoader::instance()
{
    if (!m_instance)
        m_instance = new SyntaxTemplateLoader;

    return m_instance;
}

SyntaxTemplateLoader::SyntaxTemplateLoader()
{
    loadTemplateList();
    loadTemplates();
}

void SyntaxTemplateLoader::loadTemplates()
{
    foreach(const QString &templateName,  m_templateList)
    {
        QString path = QString(DIR_SYNTAX_TEMPLATES) + "/" + templateName + ".json";

        QFile file(path);

        if(!file.open(QFile::ReadOnly))
            continue;

        QByteArray byteArray = file.readAll();
        file.close();

        QJsonObject jObject = QJsonDocument::fromJson(byteArray).object();

        QJsonObject jObjectHighlight = jObject["Highlight"].toObject();
        QJsonObject jObjectMultihighlight = jObject["Multihighlight"].toObject();
        QJsonArray jArrayCompletion = jObject["Completion"].toArray();

        foreach(const QString &key, jObjectHighlight.keys())
        {
            QJsonArray jArrayExp = jObjectHighlight[key].toArray();
            HighlightRule rule;
            rule.formatKey = key;

            foreach(const QJsonValue &jValue, jArrayExp)
            {
                rule.pattern = jValue.toString();
                m_highlightRuleListMap[templateName].append(rule);
            }
        }

        foreach(const QString &key, jObjectMultihighlight.keys())
        {
            QJsonArray jArrayExp = jObjectMultihighlight[key].toArray();
            MultihighlightRule rule;
            rule.formatKey = key;

            foreach(const QJsonValue &jValue, jArrayExp)
            {
                QJsonObject jObjectExp = jValue.toObject();
                rule.startPattern = jObjectExp["Start"].toString();
                rule.endPattern = jObjectExp["End"].toString();
                m_multihighlightRuleListMap[templateName].append(rule);
            }
        }

        foreach(const QJsonValue &value, jArrayCompletion)
        {
            CompletionData completionData;
            completionData.name = value.toString();

            m_completerDataListMap[templateName].append(completionData);
        }
    }
}

void SyntaxTemplateLoader::loadTemplateList()
{
    QDir dir(DIR_SYNTAX_TEMPLATES);
    dir.setFilter(QDir::Files);

    QStringList entryList = dir.entryList();

    foreach(const QString &name, entryList)
    {
        int size = name.size();
        int index = name.lastIndexOf('.') + 1;
        QString ext = name.right(size - index);

        if(ext == "json")
        {
            QString templateName = name.left(index - 1);
            m_templateList.append(templateName);
        }
    }
}

SyntaxTemplateLoader::CompletionDataList SyntaxTemplateLoader::completionDataList(QString templateName)
{
    return m_completerDataListMap[templateName];
}

SyntaxTemplateLoader::HighlightRuleList SyntaxTemplateLoader::highlightRuleList(QString templateName)
{
    return m_highlightRuleListMap[templateName];
}

SyntaxTemplateLoader::MultihighlightRuleList SyntaxTemplateLoader::multihighlightRuleList(QString templateName)
{
    return m_multihighlightRuleListMap[templateName];
}

QStringList SyntaxTemplateLoader::templateList()  {  return m_templateList;  }
