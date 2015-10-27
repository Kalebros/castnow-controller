#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QTextStream>
#include <QDateTime>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);

    void write(QString message);

signals:

public slots:

private:

    QTextStream *_out;
};

#endif // LOGGER_H
