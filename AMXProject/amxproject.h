#ifndef AMXPROJECT_H
#define AMXPROJECT_H

#include <baseproject.h>
#include "../FanxyStudio/projectinterface.h"

#include <QFile>
#include <QLibrary>
#include <QThread>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QProcess>

#include "codeanalyzer.h"
#include "setupwidget.h"

#if defined AMXPROJECT
 #define AMXPROJECT_COMMON_DLLSPEC Q_DECL_EXPORT
#else
 #define AMXPROJECT_COMMON_DLLSPEC Q_DECL_IMPORT
#endif

#define FILE_CONFIG_PLUGIN_SETUP    "configs/amxp_config.json"
#define FILE_CONFIG_COMPLETER       "configs/amxp_completer.json"

class AMXPROJECT_COMMON_DLLSPEC AMXProject : public BaseProject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "FanxyStudio.AMXProject" FILE "AMXProject.json")

public:
    struct FileData
    {
        QString hash;
        CodeAnalyzer::DocumentData documentData;
    };

    explicit AMXProject();
    virtual ~AMXProject();

    virtual ProjectInterface                *create();

    QList<BaseProject::CompleterInfo>       completerInfoList();

    void                                    build(BuildKey key = BUILD_RELEASE);
    void                                    setSetupData(SetupWidget::SetupData setupData);

    void                                    compile(QString path);
    bool                                    isCompileSupported() {return true;}

    QWidget                                 *setupWidget();
    //QWidget                                 *buildOutputWidget();

public slots:
    void                                    projectFileInfoUpdateRequest(int index);

protected slots:
    void                                    on_projectFileAdded(int index);
    void                                    on_projectLoaded();
    void                                    on_setupWidget_accepted(SetupWidget::SetupData setupData);

    //Build
    void                                    buildProject();
    void                                    runProject();
    //void                                    showBuildResult();
    void                                    formatBuildIssueList();

protected:
    //Config
    SetupWidget::SetupData                  m_setupData;

    //Compiler
    QProcess                                *m_compilerProcess;
    QPlainTextEdit                          *m_compileOutputWindow;

    //Build
    int                                     m_buildKey;
    QString                                 m_buildPath;
    QThread                                 *m_buildThread;
    bool                                    m_isBulding;
    bool                                    m_buildSuccessfull;

    //Completer
    QList<CompleterInfo>                    m_projectCompleterInfoList;
    QMap<int, CodeAnalyzer::DocumentData>   m_projectFileDataMap;

    //Code analyzer data file
    QFile                                   m_dataFile;

    //Widgets
    QWidget                                 *m_setupWidget;


    //Config
    void                                    loadConfigSetup();
    void                                    saveConfigSetup();

    //Compile plugin
    QString                                 compilePlugin(QString srcFile, QString outFile, QString includePath = QString());
    void                                    showCompileResult(QString output);

    //Completer
    void                                    updateProjectCompleter();
    QList<AMXProject::CompleterInfo>        getCompleterInfoList(CodeAnalyzer::DocumentData documentData);
    void                                    setupCompleter(const QMap<QString, FileData> &fileDataMap);

    //Code analyzer
    void                                    analyze(QString dataFile);
    void                                    analyzeProjectFile(int index);

    //Analyzer data storage
    QMap<QString, FileData>                 loadData(QString dataFile, const QStringList &filesList);
    void                                    saveData(QString dataFile, const QMap<QString, FileData> &fileDataMap);
    void                                    removeData(QString dataFile, const QStringList &filesList);

    //Version cheker
    QString                                 getDataVersion(QString dataFile);

    //Hash counter
    QString                                 getDataHash(QString dataFile, QString fileName);
    QString                                 getFileHash(QString path);

    //Analyzer data converter
    QJsonArray                              parsedDataListToJsonArray(CodeAnalyzer::ParsedDataList parsedDataList);
    CodeAnalyzer::ParsedDataList            jsonArrayToParsedDataList(QJsonArray jArray);

signals:
    void                                    buildFinished();
};

#endif // AMXPROJECT_H
