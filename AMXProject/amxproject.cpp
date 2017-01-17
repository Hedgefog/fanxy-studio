#include "amxproject.h"

#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QCryptographicHash>

#include <QDirIterator>

#include <QDebug>

#include <QApplication>

#if defined(Q_OS_LINUX)
    #define PATH_COMPILER               "amxxpc/amxxpc"
#elif defined(Q_OS_WIN)
    #define PATH_COMPILER               "amxxpc/amxxpc.exe"
#endif

#if defined(Q_OS_LINUX)
    #define FILE_EXECUTE_HL               "hl.sh"
#elif defined(Q_OS_WIN)
    #define FILE_EXECUTE_HL               "hl.exe"
#endif

#define DATA_VERSION_KEY   "_VERSION"
#define DATA_VERSION       "101"
#define DIR_MOD_DEFAULT             "valve"

AMXProject::AMXProject()// : BaseProject()
{
    m_isBulding = false;
    m_buildSuccessfull = false;

    m_setupWidget = NULL;
    m_buildThread = new QThread;

    m_compilerProcess = new QProcess;

    m_compileOutputWindow = new QPlainTextEdit;
    //m_compileOutputWindow->setFixedSize(640, 480);
    m_compileOutputWindow->setReadOnly(true);

    loadConfigMain();
    loadConfigSetup();

    QString dataFile = QString(FILE_CONFIG_COMPLETER);
    analyze(dataFile);

    connect(this, SIGNAL(projectFileAdded(int)), this, SLOT(on_projectFileAdded(int)));
    connect(this, SIGNAL(projectLoaded()), this, SLOT(on_projectLoaded()));

    //QString className = metaObject()->className();
    //qDebug() << "[" <<  className << "] Loaded!";

    QString userName;
#if defined(Q_OS_WIN)
userName = qgetenv("USERNAME");
#else
userName = qgetenv("USER");
#endif

    foreach(const QString &key, m_fileTypeInfoMap.keys())
       m_fileTypeInfoMap[key].fileTemplate.replace("{PLUGIN_AUTHOR}", userName);
}

AMXProject::~AMXProject()
{
    delete m_buildThread;
    delete m_compileOutputWindow;
}

ProjectInterface *AMXProject::create()
{
    ProjectInterface *created = new AMXProject;
    m_createList.append(created);
    return created;
}

void AMXProject::analyze(QString dataFile)
{
    QString dirIncludes = QFileInfo(m_setupData.compilerPath).absolutePath() + "/include";
    QStringList fileList = QDir(dirIncludes).entryList(QDir::Files);

    QString dataVersion = getDataVersion(dataFile);

    QMap<QString, FileData> fileDataMap;
    foreach(const QString &fileName, fileList)
    {
        QString path = dirIncludes + "/" + fileName;
        QString fileHash = getFileHash(path);

        if(m_dataFile.exists())
        {
            QString dataHash = getDataHash(dataFile, fileName);

            if(dataVersion == DATA_VERSION && dataHash == fileHash)
                continue;
        }

        fileDataMap[fileName].documentData = CodeAnalyzer::analyzeFile(path);
        fileDataMap[fileName].hash = fileHash;
    }

    saveData(dataFile, fileDataMap);

    fileDataMap.clear();
    fileDataMap = loadData(dataFile, fileList);

    foreach(const QString &fileName, fileDataMap.keys())
    {
        QList<CompleterInfo> completerInfoList = getCompleterInfoList(fileDataMap[fileName].documentData);
        m_completerInfoList.append(completerInfoList);
    }
}

void AMXProject::updateProjectCompleter()
{
    m_projectCompleterInfoList.clear();

    QList<int> indexList = m_projectFileDataMap.keys();
    foreach(const int &index, indexList)
    {
        QList<CompleterInfo> completerInfoList = getCompleterInfoList(m_projectFileDataMap[index]);
        m_projectCompleterInfoList.append(completerInfoList);
    }
}

QList<AMXProject::CompleterInfo> AMXProject::getCompleterInfoList(CodeAnalyzer::DocumentData documentData)
{
    QList<CompleterInfo> completerInfoList;

    foreach(const CodeAnalyzer::ParsedData &data, documentData.defines)
    {
        CompleterInfo completerInfo;
        completerInfo.info = data.value;
        completerInfo.name = data.name;

        completerInfoList.append(completerInfo);
    }

    foreach(const CodeAnalyzer::ParsedData &data, documentData.enumConstants)
    {
        CompleterInfo completerInfo;
        completerInfo.info = data.value;
        completerInfo.name = data.name;

        completerInfoList.append(completerInfo);
    }

    foreach(const CodeAnalyzer::ParsedData &data, documentData.functions)
    {
        CompleterInfo completerInfo;
        completerInfo.info = "(" + data.value + ")";
        completerInfo.name = data.name;

        completerInfoList.append(completerInfo);
    }

    return completerInfoList;
}

QMap<QString, AMXProject::FileData> AMXProject::loadData(QString dataFile, const QStringList &filesList)
{
    QFile file(dataFile);

    if(!file.open(QFile::ReadOnly))
        return QMap<QString, FileData>();

    QByteArray bytes = file.readAll();
    file.close();

    QMap<QString, FileData> fileDataMap;

    QJsonDocument jDoc = QJsonDocument::fromJson(bytes);
    QJsonObject jObject = jDoc.object();

    foreach(const QString &fileName, filesList)
    {
        QJsonObject jObjectFile = jObject[fileName].toObject();
        fileDataMap[fileName].hash = jObjectFile["Hash"].toString();

        QJsonArray jArrayDefines    = jObjectFile["Defines"].toArray();
        QJsonArray jArrayEnumConsts = jObjectFile["EnumConsts"].toArray();
        QJsonArray jArrayFunctions  = jObjectFile["Functions"].toArray();

        CodeAnalyzer::DocumentData documentData;

        documentData.defines        = jsonArrayToParsedDataList(jArrayDefines);
        documentData.enumConstants  = jsonArrayToParsedDataList(jArrayEnumConsts);
        documentData.functions      = jsonArrayToParsedDataList(jArrayFunctions);

        fileDataMap[fileName].documentData = documentData;
    }

    return fileDataMap;
}

void AMXProject::saveData(QString dataFile, const QMap<QString, FileData> &fileDataMap)
{
    QJsonDocument jDocument;
    QJsonObject jObject;

    QFile file(dataFile);

    if(file.open(QFile::ReadOnly))
    {
        QByteArray bytes = file.readAll();
        file.close();

        jDocument = QJsonDocument::fromJson(bytes);
        jObject = jDocument.object();
    }

    foreach(const QString &fileName, fileDataMap.keys())
    {
        QJsonObject jObjectFile = jObject[fileName].toObject();

        if(!jObject.isEmpty() && jObject.contains(fileName))
        {
            QString savedHash = jObjectFile["Hash"].toString();

            if(fileDataMap[fileName].hash == savedHash)
                continue;
            else
                jObject.remove(fileName);
        }

        jObjectFile["Hash"]         = fileDataMap[fileName].hash;
        jObjectFile["Defines"]      = parsedDataListToJsonArray(fileDataMap[fileName].documentData.defines);
        jObjectFile["EnumConsts"]   = parsedDataListToJsonArray(fileDataMap[fileName].documentData.enumConstants);
        jObjectFile["Functions"]    = parsedDataListToJsonArray(fileDataMap[fileName].documentData.functions);

        jObject[fileName] = jObjectFile;
    }

    jObject[DATA_VERSION_KEY] = DATA_VERSION;

    jDocument.setObject(jObject);

    if(!file.open(QFile::WriteOnly))
        return;

    file.write(jDocument.toJson());
    file.close();
}

void AMXProject::removeData(QString dataFile, const QStringList &filesList)
{
    QFile file(dataFile);

    if(!file.open(QFile::ReadOnly))
        return;

    QByteArray bytes = file.readAll();
    file.close();

    QJsonDocument jDoc = QJsonDocument::fromBinaryData(bytes);
    QJsonObject jObject = jDoc.object();

    foreach(const QString &fileName, filesList)
    {
        if(!jObject.isEmpty() && jObject.contains(fileName))
            jObject.remove(fileName);
    }

    jDoc.setObject(jObject);

    if(!file.open(QFile::WriteOnly))
        return;

    file.write(jDoc.toJson());
    file.close();
}

QJsonArray AMXProject::parsedDataListToJsonArray(CodeAnalyzer::ParsedDataList parsedDataList)
{
    QJsonArray jArray;

    foreach(const CodeAnalyzer::ParsedData &data, parsedDataList)
    {
        QJsonObject jObject;
        jObject["type"]     = data.type;
        jObject["name"]     = data.name;
        jObject["value"]    = data.value;

        jArray.append(jObject);
    }

    return jArray;
}

CodeAnalyzer::ParsedDataList AMXProject::jsonArrayToParsedDataList(QJsonArray jArray)
{
    CodeAnalyzer::ParsedDataList parsedDataList;

    foreach(const QJsonValue &jValue, jArray)
    {
        QJsonObject jObject = jValue.toObject();

        CodeAnalyzer::ParsedData parsedData;
        parsedData.type     = jObject["type"].toString();
        parsedData.name     = jObject["name"].toString();
        parsedData.value    = jObject["value"].toString();

        parsedDataList.append(parsedData);
    }

    return parsedDataList;
}

QString AMXProject::getDataHash(QString dataFile, QString fileName)
{
    QFile file(dataFile);

    if(!file.open(QFile::ReadOnly))
        return "";

    QByteArray bytes = file.readAll();
    file.close();

    QJsonDocument jDoc = QJsonDocument::fromJson(bytes);

    QJsonObject jObject = jDoc.object();
    QJsonObject jObjectFile = jObject[fileName].toObject();

    return jObjectFile["Hash"].toString();
}

QString AMXProject::getFileHash(QString path)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly))
        return "";

    QByteArray byteArray = file.readAll();

    file.close();

    return QCryptographicHash::hash(byteArray, QCryptographicHash::Md5).toHex();
}

QString AMXProject::getDataVersion(QString dataFile)
{
    QFile file(dataFile);

    if(!file.open(QFile::ReadOnly))
        return "";

    QByteArray bytes = file.readAll();
    file.close();

    QJsonObject jObject = QJsonDocument::fromJson(bytes).object();

    return jObject[DATA_VERSION_KEY].toString();
}

QList<BaseProject::CompleterInfo> AMXProject::completerInfoList()
{
    QList<CompleterInfo> list;
    list << m_completerInfoList << m_projectCompleterInfoList;
    return list;
}

void AMXProject::analyzeProjectFile(int index)
{
    QString type = projectFileType(index);

    if(type != "Includes")
        return;

    QString path = projectFilePath(index);
    QString dir = fileTypeDir(type);

    QFileInfo fileInfo(path);

    QString _path;
    if(!fileInfo.isAbsolute())
    {
        _path = m_workDir;

        if(!dir.isEmpty())
            _path += "/" + dir;

        _path += "/" + path;
    }

    m_projectFileDataMap[index] = CodeAnalyzer::analyzeFile(_path);

    updateProjectCompleter();
}

void AMXProject::projectFileInfoUpdateRequest(int index)
{
    analyzeProjectFile(index);
}

void AMXProject::on_projectFileAdded(int index)
{
    analyzeProjectFile(index);
}

void AMXProject::on_projectLoaded()
{
    QString pluginName;
    pluginName = projectName();
    pluginName = pluginName.left(pluginName.size() - pluginName.lastIndexOf('.') + 1);

    foreach(const QString &key, m_fileTypeInfoMap.keys())
        m_fileTypeInfoMap[key].fileTemplate.replace("{PLUGIN_NAME}", pluginName);
}

void AMXProject::build(BuildKey key)
{
    if(m_isBulding)
        return;

    emit projectBuildStarted();

    m_isBulding = true;
    m_buildKey = key;
    m_buildIssues.clear();

    connect(m_buildThread, SIGNAL(started()), this, SLOT(buildProject()));
    connect(m_buildThread, SIGNAL(finished()) , this, SLOT(formatBuildIssueList()));

    if(key == BUILD_RUN || key == BUILD_RUN_DEBUG)
        connect(m_buildThread, SIGNAL(finished()) , this, SLOT(runProject()));

    m_compilerProcess->moveToThread(m_buildThread);
    moveToThread(m_buildThread);

    m_buildThread->start();
}

void AMXProject::buildProject()
{
    disconnect(m_buildThread, SIGNAL(started()), this, SLOT(buildProject()));

    m_buildSuccessfull = false;

    if(m_setupData.compilerPath.isEmpty())
        return;

    m_buildPath = m_workDir;

    switch(m_buildKey)
    {
    case BUILD_RELEASE:
    case BUILD_RUN:
        m_buildPath += "/release";
        break;
    case BUILD_DEBUG:
    case BUILD_RUN_DEBUG:
        m_buildPath += "/debug";
        break;
    default:
        return;
    }

    //Remove old files
    QStringList oldFilesList;
    QDirIterator it(m_buildPath, QDirIterator::Subdirectories);
    while(it.hasNext())
        oldFilesList << it.next();

    foreach(const QString &oldFile, oldFilesList)
        QFile(oldFile).remove();

    //Make build path
    QDir().mkdir(m_buildPath);
    QDir(m_buildPath).mkpath("addons/amxmodx");
    QDir(m_buildPath).mkdir("addons/amxmodx/configs");
    QDir(m_buildPath).mkdir("addons/amxmodx/plugins");

    //Clear output buffer
    m_buildOutput.clear();

    //Config buffer
    QString projectConfig;

    //Get sources list
    QStringList sourceList = projectFileList("Sources", true);

    int currentFile = 0;
    int totalFiles = sourceList.size();

    //Compiling plugins
    foreach(QString fileNameSrc, sourceList)
    {
        //Get plugin name
        int index = fileNameSrc.lastIndexOf('.') + 1;
        QString filePathPlug = fileNameSrc.left(index) + "amxx";

        int fileNamePlugSize = filePathPlug.size() - filePathPlug.lastIndexOf('/') - 1;
        QString fileNamePlug = filePathPlug.right(fileNamePlugSize);

        //Setup variables
        QString outFile = m_buildPath + "/addons/amxmodx/plugins/" + fileNamePlug;
        QString includePath =  m_workDir + "/include";

        //Compile plugin
        QString output = compilePlugin(fileNameSrc, outFile, includePath);
        m_buildOutput.append(output);

        //Add plugin data to config buffer
        projectConfig += fileNamePlug;

        if(m_buildKey == BUILD_DEBUG || m_buildKey == BUILD_RUN_DEBUG)
            projectConfig += " debug";

        projectConfig += '\n';

        currentFile++;
        emit projectBuildStatus(currentFile, totalFiles);
    }

    //Write config
    QString projectName = m_name.left(m_name.lastIndexOf('.'));
    QString projectConfigName = "plugins-" + projectName + ".ini";
    QFile file(m_buildPath + "/addons/amxmodx/configs/" + projectConfigName);

    if(!file.open(QFile::WriteOnly))
        return;

    file.write(projectConfig.toUtf8());
    file.close();

    QRegExp regExp("([\\w|\\/|\\.|\\:|\\-|\\_]+)\\(([0-9]+)\\)\\s\\:\\s(error|fatal\\serror)\\s[0-9]+\\:[^\\n]*");
    int errorIndexInOutput = regExp.indexIn(m_buildOutput);
    m_buildSuccessfull = (errorIndexInOutput < 0);

    m_isBulding = false;

    moveToThread(qApp->thread());
    m_buildThread->exit(0);
}

void AMXProject::runProject()
{
    disconnect(m_buildThread, SIGNAL(finished()) , this, SLOT(runProject()));

    if(!m_buildSuccessfull)
        return;

    if(m_setupData.gamePath.isEmpty())
        return;

    QDir targetDir(m_buildPath);
    targetDir.setFilter(QDir::Files);

    QStringList filesList;
    QDirIterator it(targetDir, QDirIterator::Subdirectories);
    while(it.hasNext())
        filesList << it.next();

    QString modPath = m_setupData.gamePath + "/" + m_setupData.gameMod;

    foreach(const QString &sourcePath, filesList)
    {
        QString relativePath = sourcePath.right(sourcePath.length() - m_buildPath.length() - 1);
        QString targetPath = modPath + "/" + relativePath;

        if(QFile(targetPath).exists())
            QFile(targetPath).remove();

        QFile::copy(sourcePath, targetPath);
    }

    QProcess *proc = new QProcess;
    proc->setProgram(m_setupData.gamePath + "/" + FILE_EXECUTE_HL);
    proc->setWorkingDirectory(m_setupData.gamePath);
    QStringList argList;
    argList << "-game" << "cstrike" << m_setupData.executeCmd.split(" ", QString::KeepEmptyParts);
    proc->setArguments(argList);
    proc->start();
}

/*void AMXProject::showBuildResult()
{
    disconnect(m_buildThread, SIGNAL(finished()) , this, SLOT(showBuildResult()));

    showCompileResult(m_buildOutput);
}*/

void AMXProject::formatBuildIssueList()
{
    disconnect(m_buildThread, SIGNAL(finished()) , this, SLOT(formatBuildIssueList()));

    QRegExp regExp("([\\w|\\/|\\.|\\:|\\-|\\_]+)\\(([0-9]+)\\)\\s\\:\\s(warning|error|fatal\\serror)\\s[0-9]+\\:[^\\n]*");

    int index = regExp.indexIn(m_buildOutput);
    while(index >= 0)
    {
        IssueInfo issueInfo;
        issueInfo.filePath = regExp.cap(1);
        issueInfo.line = regExp.cap(2).toInt();
        issueInfo.errorString = regExp.cap(0);

        switch(regExp.cap(3)[0].toLatin1())
        {
        case 'w':
            issueInfo.type = ISSUE_WARNING;
            break;
        case 'e':
            issueInfo.type = ISSUE_ERROR;
            break;
        case 'f':
            issueInfo.type = ISSUE_FATAL_ERROR;
            break;
        }

        m_buildIssues.append(issueInfo);

        int length = regExp.matchedLength();
        index = regExp.indexIn(m_buildOutput, index + length);
    }

    emit projectBuildFinished();
}

void AMXProject::compile(QString path)
{
    m_compilerProcess->moveToThread(qApp->thread());

    int dirIndex = path.lastIndexOf('/');

    QString outputPath;

    if(m_setupData.outputPath.isEmpty() || QDir(m_setupData.outputPath).isRelative())
        outputPath = path.left(dirIndex) + "/" + m_setupData.outputPath;
    else
        outputPath = m_setupData.outputPath;

    int extIndex = path.lastIndexOf('.') + 1;

    QString srcFileName = path.right(path.size() - dirIndex);
    QString plugFileName = srcFileName.left(extIndex) + "amxx";

    outputPath += "/" + plugFileName;

    QString output = compilePlugin(path, outputPath);
    showCompileResult(output);
}

QString AMXProject::compilePlugin(QString srcFile, QString outFile, QString includePath)
{
    //Arguments
    QStringList argList;

    if(!includePath.isEmpty())
        argList << "-i" + includePath;

    if(QFile::exists(outFile))
        QFile::remove(outFile);

    argList << "-o" + outFile;
    argList << srcFile;

    //Start process
    m_compilerProcess->setArguments(argList);
    m_compilerProcess->start();
    m_compilerProcess->waitForFinished();

    return m_compilerProcess->readAllStandardOutput();
}

void AMXProject::showCompileResult(QString output)
{
    m_compileOutputWindow->setPlainText(output);
    m_compileOutputWindow->show();

    QRegExp regExp("([\\w|\\/|\\.|\\:]+)\\(([0-9]+)\\)\\s\\:\\s(warning|error|fatal\\serror)\\s[0-9]+\\:(.*)");

    QTextCharFormat format;
    format.setProperty(QTextFormat::FullWidthSelection, true);

    QList<QTextEdit::ExtraSelection> selectionList;
    int index = regExp.indexIn(m_compileOutputWindow->toPlainText());

    while(index >= 0)
    {
        QColor color;

        switch(regExp.cap(3)[0].toLatin1())
        {
        case 'w':
            color.setNamedColor("#ffc000");
            break;
        case 'e':
        case 'f':
            color.setNamedColor("#ff2020");
            break;
        }

        format.setBackground(color);

        QTextCursor textCursor = m_compileOutputWindow->textCursor();
        textCursor.setPosition(index);

        QTextEdit::ExtraSelection selection;
        selection.format = format;
        selection.cursor = textCursor;

        selectionList.append(selection);

        int length = regExp.matchedLength();
        index = regExp.indexIn(m_compileOutputWindow->toPlainText(), index + length);
    }

    m_compileOutputWindow->setExtraSelections(selectionList);
}

void AMXProject::setSetupData(SetupWidget::SetupData setupData)
{
    m_setupData = setupData;

    if(m_setupData.gameMod.isEmpty())
        m_setupData.gameMod = DIR_MOD_DEFAULT;

    if(m_setupData.compilerPath.isEmpty())
        m_setupData.compilerPath = PATH_COMPILER;

    //Get compiler path
    QString cmpPath = m_setupData.compilerPath;
    if(!QFileInfo(cmpPath).isAbsolute())
        cmpPath = QDir::currentPath() + "/" + cmpPath;

    //Process setup
    m_compilerProcess->setWorkingDirectory(QFileInfo(cmpPath).absolutePath());
    m_compilerProcess->setProgram(cmpPath);

    SetupWidget *setupWidget = dynamic_cast<SetupWidget*>(m_setupWidget);

    if(setupWidget)
        setupWidget->setSetupData(m_setupData);
}

void AMXProject::loadConfigSetup()
{
    SetupWidget::SetupData setupData;

    QByteArray byteArray;
    QFile file(FILE_CONFIG_PLUGIN_SETUP);
    if(file.open(QFile::ReadOnly))
    {
        byteArray = file.readAll();
        file.close();

        if(!byteArray.isEmpty())
        {
            QJsonDocument jDocument = QJsonDocument::fromJson(byteArray);
            QJsonObject jObject = jDocument.object();

            if(jObject.isEmpty())
                return;


            setupData.compilerPath    = jObject["CompilerPath"].toString();
            setupData.gamePath        = jObject["GamePath"].toString();
            setupData.gameMod         = jObject["Mod"].toString();
            setupData.executeCmd      = jObject["ExecuteCommand"].toString();
            setupData.outputPath      = jObject["OutputPath"].toString();
        }
    }

    setSetupData(setupData);
}

void AMXProject::saveConfigSetup()
{
    QJsonObject jObject;
    jObject["CompilerPath"]     = m_setupData.compilerPath;
    jObject["GamePath"]         = m_setupData.gamePath;
    jObject["Mod"]              = m_setupData.gameMod;
    jObject["ExecuteCommand"]   = m_setupData.executeCmd;
    jObject["OutputPath"]       = m_setupData.outputPath;

    QFile file(FILE_CONFIG_PLUGIN_SETUP);

    if(!file.open(QFile::WriteOnly))
        return;

    file.write(QJsonDocument(jObject).toJson());
    file.close();
}

QWidget *AMXProject::setupWidget()
{
    if(!m_setupWidget)
    {
        SetupWidget *setupWidget = new SetupWidget;
        setupWidget->setSetupData(m_setupData);
        connect(setupWidget, SIGNAL(accepted(SetupWidget::SetupData)), this, SLOT(on_setupWidget_accepted(SetupWidget::SetupData)));
        m_setupWidget = setupWidget;
    }

    return m_setupWidget;
}

//QWidget *AMXProject::buildOutputWidget()
//{
//    return m_compileOutputWindow;
//}

void AMXProject::on_setupWidget_accepted(SetupWidget::SetupData setupData)
{
    setSetupData(setupData);
    saveConfigSetup();

    foreach(ProjectInterface *project, m_createList)
    {
        AMXProject *amxProject = dynamic_cast<AMXProject*>(project);

        if(amxProject)
            amxProject->setSetupData(m_setupData);
    }
}
