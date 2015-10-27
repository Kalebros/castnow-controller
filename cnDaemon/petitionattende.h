#ifndef PETITIONATTENDE_H
#define PETITIONATTENDE_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class CNDaemon;

class PetitionAttende : public QObject
{
    Q_OBJECT
public:
    explicit PetitionAttende(QObject *parent = 0, QTcpSocket *s=0, CNDaemon *d=0);

signals:

    void workDone();
    void messageToDaemon(QString m);

public slots:

    void informationReady();

private:
    QTcpSocket *_socket;
    CNDaemon *_daemon;

    int _bytesAvailable;
    bool _isCommandRead;
    bool _isCommandAttend;
    bool _isSizeRead;

    quint16 _commandSize;

    QString _command;
    QJsonDocument _jsonCommand;
    QJsonDocument _jsonResponse;

    QDataStream *stream;

    QByteArray attendCommand();

    QByteArray attendList();

    QByteArray attendPlay();

    QString whichCommand() const;
};

#endif // PETITIONATTENDE_H
