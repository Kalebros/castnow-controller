#ifndef CNCONTPETITION_H
#define CNCONTPETITION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

class CNContPetition : public QObject
{
    Q_OBJECT
public:
    explicit CNContPetition(QObject *parent = 0,QString type=QString(),QHostAddress address=QHostAddress(),int port=0);

    QString type() const
    { return _type; }

    void start();

signals:

    void petitionReady();

public slots:

private:

    QString _type;

protected:

    QTcpSocket *_socket;
    QHostAddress _address;
    int _port;
    QDataStream *stream;

protected slots:

    virtual void startPetition()=0;
    virtual void infoReady()=0;
};

#endif // CNCONTPETITION_H
