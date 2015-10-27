#ifndef CNPROCESS_H
#define CNPROCESS_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QTextStream>

#include <QDebug>
#include <QHostAddress>

#include "cacheentry.h"


class CNProcess : public QObject
{
    Q_OBJECT

public:

    enum Status
    { Idle=0,Playing,Paused };
    Q_ENUM(Status)

    explicit CNProcess(QObject *parent = 0);

    void setAddress(QHostAddress address)
    { _address=address; }

    CNProcess::Status currentStatus() const;
    QJsonObject currentStatusJson() const;

    CacheEntry *getCurrentEntry() const
    { return _currentEntry; }

    QString getCurrentSubtitle() const
    { return _currentSubtitle; }

signals:

    void statusChanged(CNProcess::Status status);
    void toLog(QString message);

public slots:

    void play(CacheEntry *entry,QString subtitle);
    void stop();
    void pause();

private:

    CNProcess::Status _status;
    CacheEntry *_currentEntry;
    QString _currentSubtitle;
    QHostAddress _address;

    QProcess _castnow;
    QTextStream *_cnStream;

private slots:

    void errorProcess(QProcess::ProcessError error);

};

#endif // CNPROCESS_H
