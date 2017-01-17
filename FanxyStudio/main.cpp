#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDir>

#include "logger.h"
#include <QDebug>

#define FS_DEPLOY

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if defined(Q_OS_LINUX)
    QDir::setCurrent(a.applicationDirPath());
#elif defined(Q_OS_WIN)
    #if defined(FS_DEPLOY)
        QDir::setCurrent(a.applicationDirPath());
    #endif
#endif

    QDir().mkdir("./configs");

    MainWindow w;
    w.show();

    if(argc > 1)
    {
        QString path = QString(argv[1]);
        if(QFile(path).exists())
            w.fileOpen(path);
    }

    int result = a.exec();

    Logger::write("Application exited with code " + QString::number(result));

    return result;
}
