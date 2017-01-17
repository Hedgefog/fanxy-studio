#ifndef CODEANALYZER_H
#define CODEANALYZER_H

#include <QObject>

class CodeAnalyzer
{
public:
    enum
    {
        PARSESTAGE_NAME_PRE = 0,
        PARSESTAGE_NAME,
        PARSESTAGE_NAME_POST,
        PARSESTAGE_VALUE_PRE,
        PARSESTAGE_VALUE,
        PARSESTAGE_VALUE_POST,
        PARSESTAGE_END,
        PARSESTAGE_END_EMERGENCY
    };

    struct ParsedData
    {
        QString type;
        QString name;
        QString value;
    };

    typedef QList<ParsedData> ParsedDataList;

    struct DocumentData
    {
        ParsedDataList enumConstants;
        ParsedDataList functions;
        ParsedDataList defines;
    };

    static DocumentData analyzeFile(QString path);
    static DocumentData analyzeData(QString buffer);

protected:
    static ParsedDataList getEnumConstants(QString buffer);
    static ParsedDataList getFunctions(QString buffer);
    static ParsedDataList getDefines(QString buffer);
    static void removeComments(QString &buffer);
};

#endif // CODEANALYZER_H
