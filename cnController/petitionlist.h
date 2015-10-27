#ifndef PETITIONLIST_H
#define PETITIONLIST_H

#include <QDataStream>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "cncontpetition.h"

class PetitionList : public CNContPetition
{
    Q_OBJECT
public:
    explicit PetitionList(QObject *parent = 0,QHostAddress address=QHostAddress(),int port=0);

    void startPetition();

    QJsonDocument getList() const
    { return doc; }
signals:

public slots:

protected slots:

    void infoReady();

private:

    bool _isSizeOfListReady;
    int _bytesAvailable;

    quint16 _sizeOfList;

    QByteArray _fileList;

    QString _list;
    QJsonDocument doc;
};

#endif // PETITIONLIST_H
