#ifndef CNCONTROLLER_H
#define CNCONTROLLER_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDataStream>
#include <QFontDatabase>
#include <QStringList>

#include "cncontpetition.h"
#include "petitionlist.h"
#include "petitionplay.h"
#include "cnfileentry.h"

class CNController : public QObject
{
    Q_OBJECT
public:
    explicit CNController(QObject *parent = 0);

    Q_INVOKABLE void searchServer();
    Q_INVOKABLE void askForFiles();
    Q_INVOKABLE void playFile(int id,QString subtitle);

    enum CNInstructions
    { BEGIN=0x00,END=0xff,STATUS=0x01,LIST=0x02,PLAY=0x04,STOP=0x08,PAUSE=0x10 };

signals:

    void serverFound(QString sName,int port);

    void fileList(QList<CNFileEntry*> list);

public slots:

    void discoverResponse();

private:

    QUdpSocket discoverSocket;
    QHostAddress address;
    unsigned short int port;
    QString serverName;
    QJsonDocument _filesList;

    CNContPetition *_currentPetition;

private slots:

    void currentPetitionEnded();

};

#endif // CNCONTROLLER_H
