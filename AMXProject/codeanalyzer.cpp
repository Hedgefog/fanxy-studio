#include "codeanalyzer.h"

#include <QString>
#include <QFile>

CodeAnalyzer::DocumentData CodeAnalyzer::analyzeFile(QString path)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly))
        return DocumentData();

    QString buffer = file.readAll();

    file.close();

    return analyzeData(buffer);
}

CodeAnalyzer::DocumentData CodeAnalyzer::analyzeData(QString buffer)
{
    DocumentData documentData;

    removeComments(buffer);

    if(buffer.isEmpty())
        return DocumentData();

    documentData.defines = getDefines(buffer);
    documentData.enumConstants = getEnumConstants(buffer);
    documentData.functions = getFunctions(buffer);

    return documentData;
}

CodeAnalyzer::ParsedDataList CodeAnalyzer::getDefines(QString buffer)
{
    QString type = "#define";

    ParsedDataList dataList;
    int length = buffer.length();

    QStringList list;

    int startIndex = 0;

    while(startIndex < length)
    {
        startIndex = buffer.indexOf(type, startIndex);

        if(startIndex == -1)
            break;

        if(startIndex == length - 1)
            break;

        QChar preChar;
        QChar postChar;

        bool inStart = (startIndex == 0);

        if(!inStart)
            preChar = buffer[startIndex - 1];

        startIndex += type.length();

        if(startIndex >= length)
            break;

        if(!inStart && preChar != QChar::Space && preChar != '\t' && preChar != '\n')
            continue;

        postChar = buffer[startIndex];

        if(postChar != QChar::Space && postChar != '\t' && postChar != '\n')
            continue;

        startIndex++;

        ParsedData data;
        data.type = type;
        int parseStage = PARSESTAGE_NAME_PRE;

        for(int i = startIndex; i < length; i++)
        {
            char ch = buffer[i].toLatin1();

            if(ch == '\n')
                parseStage = PARSESTAGE_END;

            if(ch == ' ' || ch == '\n' || ch == '\t')
            {
                if(parseStage == PARSESTAGE_NAME_PRE)
                    continue;

                if(parseStage == PARSESTAGE_NAME_POST)
                    continue;

                if(parseStage == PARSESTAGE_NAME)
                {
                    parseStage = PARSESTAGE_NAME_POST;
                    continue;
                }
            }

            switch(parseStage)
            {
            case PARSESTAGE_NAME_PRE:
                parseStage = PARSESTAGE_NAME;
            case PARSESTAGE_NAME:
                data.name += ch;

                break;
            case PARSESTAGE_NAME_POST:
                parseStage = PARSESTAGE_VALUE;
            case PARSESTAGE_VALUE:
                data.value += ch;

                break;
            }

            if(i == length - 1)
                parseStage = PARSESTAGE_END;

            if(parseStage == PARSESTAGE_END)
            {
                if(!data.name.isEmpty())
                    dataList.append(data);

                data.name.clear();
                data.value.clear();
                break;
            }
        }
    }

    return dataList;
}

CodeAnalyzer::ParsedDataList CodeAnalyzer::getFunctions(QString buffer)
{
    QStringList types;
    types << "stock" << "public" << "native" << "forward";

    ParsedDataList dataList;
    int length = buffer.length();

    foreach(const QString &type, types)
    {
        int startIndex = 0;

        while(startIndex < length)
        {
            startIndex = buffer.indexOf(type, startIndex);

            if(startIndex == -1)
                break;

            if(startIndex == length - 1)
                break;

            QChar preChar;
            QChar postChar;

            bool inStart = (startIndex == 0);

            if(!inStart)
                preChar = buffer[startIndex - 1];

            startIndex += type.length();

            if(startIndex >= length)
                break;

            if(!inStart && preChar != QChar::Space && preChar != '\t' && preChar != '\n' && preChar != ';')
                continue;

            postChar = buffer[startIndex];

            if(postChar != QChar::Space && postChar != '\t' && postChar != '\n')
                continue;

            startIndex++;

            ParsedData data;
            data.type = type;
            int parseStage = PARSESTAGE_NAME_PRE;

            for(int i = startIndex; i < length; i++)
            {
                char ch = buffer[i].toLatin1();

                if(ch == ' ' || ch == '\n' || ch == '\t')
                {
                    if(parseStage == PARSESTAGE_NAME_PRE)
                        continue;

                    if(parseStage == PARSESTAGE_NAME_POST)
                        continue;

                    if(parseStage == PARSESTAGE_NAME)
                    {
                        parseStage = PARSESTAGE_NAME_POST;
                        continue;
                    }
                }

                switch(parseStage)
                {
                case PARSESTAGE_NAME_PRE:
                    parseStage = PARSESTAGE_NAME;
                case PARSESTAGE_NAME:
                    if(ch == '(')
                        parseStage = PARSESTAGE_VALUE;
                    else if(ch == '=')
                        parseStage = PARSESTAGE_END_EMERGENCY;
                    else if(ch == ':')
                        data.name.clear();
                    else
                        data.name += ch;

                    break;
                case PARSESTAGE_NAME_POST:
                    if(ch == '(')
                        parseStage = PARSESTAGE_VALUE;
                    else
                        parseStage = PARSESTAGE_END_EMERGENCY;

                    break;
                case PARSESTAGE_VALUE:
                    if(ch == ')')
                        parseStage = PARSESTAGE_END;
                    else
                        data.value += ch;

                    break;
                }

                if(parseStage == PARSESTAGE_END_EMERGENCY)
                    break;

                if(parseStage == PARSESTAGE_END)
                {
                    if(!data.name.isEmpty())
                        dataList.append(data);

                    data.name.clear();
                    data.value.clear();
                    break;
                }
            }
        }
    }

    return dataList;
}

CodeAnalyzer::ParsedDataList CodeAnalyzer::getEnumConstants(QString buffer)
{
    QString type = "enum";
    ParsedDataList dataList;
    int length = buffer.length();

    int startIndex = 0;
    int endIndex = 0;

    while(startIndex < length)
    {
        startIndex = buffer.indexOf(type, endIndex);

        if(startIndex == -1)
            break;

        startIndex = buffer.indexOf('{', startIndex);

        if(startIndex == -1)
            break;

        endIndex = buffer.indexOf('}', startIndex);

        if(endIndex == -1)
            break;

        startIndex++;
        endIndex--;

        int enumLastValue = 0;
        ParsedData data;
        data.type = type;

        int parseStage = PARSESTAGE_NAME_PRE;

        for(int i = startIndex; i <= endIndex; i++)
        {
            char ch = buffer[i].toLatin1();

            if(ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
            {
                if(i == endIndex)
                    parseStage = PARSESTAGE_END;

                if(parseStage == PARSESTAGE_NAME_PRE)
                    continue;

                if(parseStage == PARSESTAGE_NAME_POST)
                    continue;

                if(parseStage == PARSESTAGE_VALUE_PRE)
                    continue;

                if(parseStage == PARSESTAGE_VALUE_POST)
                    continue;

                if(parseStage == PARSESTAGE_VALUE)
                {
                    parseStage = PARSESTAGE_VALUE_POST;
                    continue;
                }

                if(parseStage == PARSESTAGE_NAME)
                {
                    parseStage = PARSESTAGE_NAME_POST;
                    continue;
                }
            }

            if(ch == ',')
                parseStage = PARSESTAGE_END;

            switch(parseStage)
            {
            case PARSESTAGE_NAME_PRE:
                parseStage = PARSESTAGE_NAME;
            case PARSESTAGE_NAME:
                if(ch == '=')
                    parseStage = PARSESTAGE_VALUE_PRE;
                else if(ch == ':')
                    data.name.clear();
                else
                    data.name += ch;

                break;
            case PARSESTAGE_NAME_POST:
                if(ch == '=')
                    parseStage = PARSESTAGE_VALUE_PRE;
                else
                    parseStage = PARSESTAGE_END_EMERGENCY;

                break;
            case PARSESTAGE_VALUE_PRE:
                parseStage = PARSESTAGE_VALUE;
            case PARSESTAGE_VALUE:
                data.value += ch;

                break;
            }

            if(i == endIndex)
                parseStage = PARSESTAGE_END;

            if(parseStage == PARSESTAGE_END_EMERGENCY)
                break;

            if(parseStage == PARSESTAGE_END)
            {
                if(!data.value.isEmpty())
                    enumLastValue = QString(data.value).toInt();
                else
                    enumLastValue++;

                data.value = QString::number(enumLastValue);

                if(!data.name.isEmpty())
                    dataList.append(data);

                data.name.clear();
                data.value.clear();
                parseStage = PARSESTAGE_NAME_PRE;
            }
        }
    }

    return dataList;
}

void CodeAnalyzer::removeComments(QString &buffer)
{
    if(buffer.isEmpty())
        return;

    bool mlOpened = false;
    bool slOpened = false;

    int startPos = 0;
    int length = buffer.length();

    for(int i = startPos; i < length; i++)
    {
        QString mid = buffer.mid(i, 2);

        if(mid.isEmpty() || mid.length() != 2)
            continue;

        if(!mlOpened && !slOpened)
        {
            if(mid == "/*")
                mlOpened = true;
            else if(mid == "//")
                slOpened = true;

            if(mlOpened || slOpened)
            {
                startPos = i;
                i++;
            }
        }
        else
        {
            int size = 0;

            if(mlOpened)
            {
                if(mid == "*/")
                {
                    mlOpened = false;
                    size = 2;
                }
            }
            else
            {
                if(mid.contains('\n'))
                {
                    slOpened = false;

                    if(mid[0] == '\n')
                        size = 0;
                    else
                        size = 1;
                }
            }

            if(!mlOpened && !slOpened)
            {
                buffer.remove(startPos, i - startPos + size);
                i = startPos;
            }
        }
    }

    if(slOpened)
        buffer.remove(startPos, length);
}
