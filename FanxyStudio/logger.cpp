#include "logger.h"

#include <QFile>
#include <QDir>
#include <QDateTime>

void Logger::write(QString text)
{
    if(!QDir("logs").exists())
        QDir().mkdir("logs");

    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.date().toString("yyyyMMdd");
    QString time = dateTime.time().toString("[HH:mm]");

    QString path = "logs/" + date + ".log";

    QFile file(path);

    if(!file.open(QFile::Append))
        return;

    file.write(QString(time + text + "\r\n").toUtf8());
    file.close();
}
