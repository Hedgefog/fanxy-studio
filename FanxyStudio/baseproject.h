#ifndef BASEPROJECT_H
#define BASEPROJECT_H

#include <QObject>
#include <QtPlugin>
#include <QMap>

#define DIR_CONFIG                  "configs"

#define BaseProject_iid "FanxyStudio.BaseProjectInterface"

class BaseProject : public QObject
{
    Q_OBJECT
public:
    struct FILETYPEINFO
    {
        QString extension;
        QString dir;
    };

    struct COMPLETERINFO
    {
        QString name;
        QString info;
    };


    explicit BaseProject();
    virtual ~BaseProject() {}

    virtual QStringList                     getFileTypeList();

    virtual QString                         fileTypeByFileName(QString name);
    virtual FILETYPEINFO                    fileTypeInfo(QString type);

    virtual QString                         name();
    virtual QString                         extension();
    virtual QString                         typeDefault();
    virtual QMap<QString, FILETYPEINFO>     fileTypeInfoMap();
    virtual QString                         highlightTemplate();
    virtual QList<COMPLETERINFO>            completerInfoList();

    virtual void                            compile();
    virtual void                            compileDebug();
    virtual void                            run();
    virtual void                            runDebug();

protected:
    QString                                 m_name;
    QString                                 m_extension;
    QString                                 m_typeDefault;
    QMap<QString, FILETYPEINFO>             m_fileTypeInfoMap;
    QString                                 m_highlightTemplate;
    QList<COMPLETERINFO>                    m_completerInfoList;

    virtual bool                            load();
    virtual bool                            loadConfigMain();
    virtual bool                            loadConfigCompleter();

};

Q_DECLARE_INTERFACE(BaseProject, BaseProject_iid)

#endif // BASEPROJECT_H
