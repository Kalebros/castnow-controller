#ifndef PETITIONPLAY_H
#define PETITIONPLAY_H

#include <QDataStream>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "cncontpetition.h"

class PetitionPlay : public CNContPetition
{
    Q_OBJECT
public:
    explicit PetitionPlay(QObject *parent = 0,QHostAddress address=QHostAddress(),int port=0,int id=-1,QString subtitle=QString());

    void setMovie(int idMovie,QString subtitle);

    QJsonDocument getStatus();

signals:

public slots:

protected slots:

    void startPetition();
    void infoReady();

protected:

    bool _isSizeOfResponseReady;
    int _bytesAvailable;
    int _sizeResponse;

    int _id;
    QString _subtitle;

    QByteArray buildJsonPetition();
    QJsonDocument doc;
};

#endif // PETITIONPLAY_H
