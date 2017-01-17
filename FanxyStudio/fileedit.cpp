#include "fileedit.h"

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractTextDocumentLayout>

#include "logger.h"

FileEdit::FileEdit(QWidget *parent) : QWidget(parent)
{
    m_tabBar = new QTabBar;
    m_tabBar->setTabsClosable(true);
    m_tabBar->setMovable(true);

    m_codeEdit = new CodeEdit;

    m_defTextDocument = new QTextDocument;
    QPlainTextDocumentLayout *defTextDocumentLayout = new QPlainTextDocumentLayout(m_defTextDocument);
    m_defTextDocument->setDocumentLayout(defTextDocumentLayout);

    m_layout = new QVBoxLayout;
    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    m_layout->insertWidget(0, m_tabBar);
    m_layout->insertWidget(1, m_codeEdit);

    connect(m_tabBar, SIGNAL(currentChanged(int)), this, SLOT(on_tabChanged(int)));
    connect(m_tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tabCloseRequested(int)));
    connect(m_tabBar, SIGNAL(tabMoved(int, int)), this, SLOT(on_tabMoved(int, int)));

    setLayout(m_layout);

    hide();
}

FileEdit::~FileEdit()
{
    delete m_tabBar;
    delete m_codeEdit;
    delete m_defTextDocument;
    delete m_layout;
}


bool FileEdit::isFileOpened(QString path)
{
    return (getFileIndex(path) >= 0);
}

int FileEdit::getFileIndex(QString path)
{
    int documentCount = m_documentVector.count();

    if(documentCount <= 0)
        return -1;

    for(int i = 0; i < documentCount; i++)
    {
        if(m_documentVector[i]->path == path)
            return i;
    }

    return -1;
}


QString FileEdit::filePath(int index)
{
    int documentCount = m_documentVector.count();

    if(documentCount <= 0)
        return QString();

    if(index >= documentCount)
        return QString();

    return m_documentVector[index]->path;
}

//File new

bool FileEdit::fileNew()
{
    Document *document = new Document;
    QTextDocument *textDocument = new QTextDocument;

    QPlainTextDocumentLayout *documentLayout = new QPlainTextDocumentLayout(textDocument);
    textDocument->setDocumentLayout(documentLayout);

    document->name = "New File";
    document->textDocument = textDocument;

    int index = documentAdd(document);

    if(index < 0)
        return false;

    m_tabBar->setCurrentIndex(index);

    Logger::write("New file created.");

    return true;
}

//File open main method

bool FileEdit::fileOpen(QString path)
{
    int index;

    if(!isFileOpened(path))
    {
        QFile file(path);
        bool isOpened = file.open(QFile::ReadOnly);

        if(!isOpened)
            return false;

        QByteArray byteArray = file.readAll();
        file.close();

        QTextDocument *textDocument = new QTextDocument;
        textDocument->setPlainText(byteArray);
        textDocument->setModified(false);

        QPlainTextDocumentLayout *documentLayout = new QPlainTextDocumentLayout(textDocument);
        textDocument->setDocumentLayout(documentLayout);

        QFileInfo fileInfo(path);
        QString fileName = fileInfo.fileName();

        Document *document = new Document;
        document->textDocument = textDocument;
        document->path = path;
        document->name = fileName;

        index = documentAdd(document);

        if(index < 0)
            return false;
    }
    else
    {
       index = getFileIndex(path);
    }

    m_tabBar->setCurrentIndex(index);
    m_documentVector[index]->textDocument->setModified(false);

    emit fileOpened(index);
    emit fileOpened(path);

    Logger::write("File opened.");

    return true;
}

//File open methods

bool FileEdit::fileOpen()
{
    //Setup filter
    QString filter = "All types (*.*)";

    //Open file dialog
    QString path = QFileDialog::getOpenFileName
    (
        this,
        "Open file...",
        QString(),
        filter.toUtf8()
    );

    if(path.isEmpty())
        return false;

    return fileOpen(path);
}

//File close main method

bool FileEdit::fileClose(int index)
{
    if(index < 0)
        return false;

    Document *document = m_documentVector[index];

    if(!document)
        return false;

    QTextDocument *textDocument = document->textDocument;

    if(!textDocument || textDocument == m_defTextDocument)
        return false;

    if(textDocument->isModified())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("The " + document->name + " has been modified.");
        msgBox.setText("Do you want to save your changes in " + document->name + "?");
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if(msgBox.exec() == QMessageBox::Yes)
        {
            if(!document->path.isEmpty())
                fileSave(index);
            else
                fileSaveAs(index);
        }
    }

    m_documentVector.removeAt(index);
    m_tabBar->removeTab(index);
    if(m_tabBar->count() < 1)
        m_codeEdit->setDocument(m_defTextDocument);

    delete document;
    delete textDocument;

    emit fileClosed(index);
    //emit fileClosed(path);

    Logger::write("File closed");

    return true;
}

//File close methods

bool FileEdit::fileClose()
{
    return fileClose(index());
}

void FileEdit::fileCloseAll()
{
    int documentCount = m_documentVector.count();

    if(documentCount <= 0)
        return;

    for(int i = documentCount - 1; i >= 0; i--)
        fileClose(i);
}

//File save main method

bool FileEdit::fileSave(int index, QString path)
{
    if(index < 0)
        return false;

    if(path.isEmpty())
        return fileSaveAs(index);

    Document *document = m_documentVector[index];

    if(!document)
        return false;

    QTextDocument *textDocument = document->textDocument;

    if(!textDocument || textDocument == m_defTextDocument)
        return false;

    QFile file(path);

    if(!file.open(QFile::WriteOnly))
        return false;

    file.write(textDocument->toPlainText().toUtf8());
    file.close();

    textDocument->setModified(false);

    emit fileSaved(index);
    emit fileSaved(path);

    Logger::write("File saved.");

    return true;
}

//File save methods

bool FileEdit::fileSave()
{
    int index = this->index();

    return fileSave(index);
}

bool FileEdit::fileSave(int index)
{
    if(index < 0)
        return false;

    Document *document = m_documentVector[index];

    if(!document)
        return false;

    return fileSave(index, document->path);
}

//File save as methods

bool FileEdit::fileSaveAs()
{
    int index = this->index();

    return fileSaveAs(index);
}

bool FileEdit::fileSaveAs(int index)
{
    if(index < 0)
        return false;

    QString filter = "All types (*.*)";

    QString path = QFileDialog::getSaveFileName
    (
        this,
        "Save file as...",
        QString(),
        tr(filter.toUtf8())
    );

    if(path.isEmpty())
        return false;

    QFileInfo fileInfo(path);
    QString fileName = fileInfo.fileName();

    m_documentVector[index]->path = path;
    m_documentVector[index]->name = fileName;
    m_tabBar->setTabText(index, fileName);
    m_tabBar->setTabToolTip(index, path);

    return fileSave(index, path);
}

//Document methods

int FileEdit::documentAdd(Document *document)
{
    int index = m_documentVector.count();

    m_documentVector.insert(index, document);
    m_tabBar->insertTab(index, document->name);
    m_tabBar->setTabToolTip(index, document->path);

    connect(document->textDocument, SIGNAL(modificationChanged(bool)), this, SLOT(on_document_modificationChanged(bool)));

    return index;
}

//Slots

void FileEdit::on_tabChanged(int index)
{
    QTextDocument *textDocument = NULL;

    if(isHidden())
        show();

    if(!m_documentVector.isEmpty() && index >= 0)
        textDocument = m_documentVector[index]->textDocument;
    else
        hide();

    if(textDocument)
        m_codeEdit->setDocument(textDocument);
}

void FileEdit::on_tabCloseRequested(int index)
{
    fileClose(index);
}

void FileEdit::on_tabMoved(int from, int to)
{
    Document *tmpDocument = m_documentVector[from];
    m_documentVector.replace(from, m_documentVector[to]);
    m_documentVector.replace(to, tmpDocument);
}

void FileEdit::on_document_modificationChanged(bool value)
{
    QTextDocument *textDocument = dynamic_cast<QTextDocument*>(sender());

    if(!textDocument || textDocument == m_defTextDocument)
        return;

    int index = this->index();

    Document *document = m_documentVector[index];

    if(textDocument != document->textDocument)
        return;

    QString text = document->name;

    if(value)
        text += "*";

     m_tabBar->setTabText(index, text);
}

//Get methods

int                 FileEdit::index()                {   return m_tabBar->currentIndex();    }
CodeEdit            *FileEdit::codeEdit()            {   return m_codeEdit;                  }

QTextDocument       *FileEdit::document()               {  return documentAt(this->index());                   }
QTextDocument       *FileEdit::documentAt(int index)    {  return m_documentVector[index]->textDocument;       }
