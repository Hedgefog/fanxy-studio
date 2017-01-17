#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QObject>
#include <QWidget>
#include <QSyntaxHighlighter>
#include <QPlainTextEdit>

#include "skinloader.h"
#include "settingloader.h"
#include "syntaxtemplateloader.h"

#define HIGHLIGHT_TEMPLATE_NONE   "None"

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

protected : struct Border
{
    int pos1;
    int pos2;
    QString ownerName;
};

public:
    SyntaxHighlighter(QTextDocument *parent = 0);

    void                    setSyntaxTemplate(QString templateName);
//    void                    setHighlightList(QStringList highlightList);

protected:
    void                    highlightBlock(const QString &text);
    void                    highlightSingleLine(const QString &text);
    void                    highlightMultiLine(const QString &text);

    SyntaxTemplateLoader    *m_syntaxTemplateLoader;
    SettingLoader           *m_settingLoader;
    SkinLoader              *m_skinLoader;
    QString                 m_syntaxTemplate;
    QStringList             m_highlightList;

protected slots:
    void                    on_skinLoader_loaded();
};

#endif // SYNTAXHIGHLIGHTER_H
