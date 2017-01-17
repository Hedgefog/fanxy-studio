#ifndef FILEEDIT_H
#define FILEEDIT_H

#include <QWidget>
#include <QTabBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextDocument>

#include "codeedit.h"

#include "linenumberwidget.h"

class FileEdit : public QWidget
{
    Q_OBJECT

public : struct Document
{
    QTextDocument *textDocument;
    QString path;
    QString name;
    bool isModified;
};

public:
    explicit FileEdit(QWidget *parent = 0);
    ~FileEdit();

    bool                isFileOpened(QString path);
    int                 getFileIndex(QString path);
    int                 index();

    LineNumberWidget    *lineNumberWidget();
    CodeEdit            *codeEdit();

    QTextDocument       *document();
    QTextDocument       *documentAt(int index);


public slots:
    bool                fileNew();

    bool                fileOpen();
    bool                fileOpen(QString path);

    bool                fileClose();
    bool                fileClose(int index);

    void                fileCloseAll();

    bool                fileSave(int index, QString path);

    bool                fileSaveAs();
    bool                fileSaveAs(int index);

    bool                fileSave();
    bool                fileSave(int index);

    QString             filePath(int index);

protected:
    int                 documentAdd(Document *document);
    QTabBar             *m_tabBar;
    CodeEdit            *m_codeEdit;
    QVBoxLayout         *m_layout;
    QVector<Document*>  m_documentVector;
    QTextDocument       *m_defTextDocument;

private slots:
    void                on_tabChanged(int index);
    void                on_tabCloseRequested(int index);
    void                on_document_modificationChanged(bool value);
    void                on_tabMoved(int from, int to);

signals:
    void                fileSaved(int index);
    void                fileClosed(int index);
    void                fileOpened(int index);

    void                fileSaved(QString path);
    void                fileClosed(QString path);
    void                fileOpened(QString path);
};

#endif // FILEEDIT_H
