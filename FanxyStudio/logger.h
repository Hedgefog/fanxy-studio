#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

class Logger
{
public:
    static void write(QString text);
};

#endif // LOGGER_H
