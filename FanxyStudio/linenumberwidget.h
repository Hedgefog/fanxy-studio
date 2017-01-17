#ifndef LINENUMBERWIDGET_H
#define LINENUMBERWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QGraphicsView>
#include <QScrollArea>
#include <QTextDocument>

#include "settingloader.h"
#include "skinloader.h"

class LineNumberWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LineNumberWidget(QWidget *parent = 0);
    ~LineNumberWidget();

    void            setEditor(QPlainTextEdit *editor);

public slots:
    void            updateWidth();

protected:
    void            paintEvent(QPaintEvent *event);

    QPlainTextEdit  *m_editor;
    SettingLoader   *m_settingLoader;
    SkinLoader      *m_skinLoader;

protected slots:
    void            on_editor_vScrollBar_valueChanged(int);

signals:
    void            widthChanged(int width);
};

#endif // LINENUMBERWIDGET_H
