#ifndef SKINLOADER_H
#define SKINLOADER_H

#include <QObject>
#include <QMap>
#include <QColor>
#include <QFont>
#include <QTextCharFormat>

#define DIR_SKINS               "skins"
#define DIR_SKINS_ICONS         "icons"
#define FILE_SKIN_CONFIG        "config.json"
#define FILE_SKIN_STYLE_SYNTAX  "syntax.json"
#define FILE_SKIN_STYLE_EDITOR  "editor.json"
#define FILE_SKIN_STYLESHEET    "style.css"

class SkinLoader : public QObject
{
    Q_OBJECT

public : typedef QMap<QString, QTextCharFormat> StyleMap;

public:
    static SkinLoader *instance();
    explicit SkinLoader(QObject *parent = 0);

    void                            load(QString name);

    QString                         iconsPack();
    QString                         styleSheet();
    QTextCharFormat                 syntaxFormat(const QString key);
    QTextCharFormat                 editorFormat(const QString key);

    QStringList                     skinList();

protected:
    void                            loadConfig(QString path);
    void                            loadStyleSheet(QString path);
    void                            loadSyntaxFormat(QString path);
    void                            loadEditorFormat(QString path);
    void                            loadSkinList();

    bool                            isValidColorName(QString colorName);
    QMap<QString, QTextCharFormat>  getFormatMap(QString path);
    QTextCharFormat                 formatFromJsonObject(const QJsonObject &jObject);

    static SkinLoader               *m_instance;

    QString                         m_iconsPack;
    QString                         m_styleSheet;
    QMap<QString, QTextCharFormat>  m_syntaxFormatMap;
    QMap<QString, QTextCharFormat>  m_editorFormatMap;
    QStringList                     m_skinList;

signals:
   void loaded();
};

#endif // SKINLOADER_H
