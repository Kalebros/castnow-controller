//////////////////////////////////////////////////////////////////////////////
///   Castnow-controller
///   A simple C++ castnow daemon and a QML-based controller
///   Copyright (C) 2015 Antonio Ramírez Marti (morgulero@gmail.com)
///
///   This program is free software: you can redistribute it and/or modify
///   it under the terms of the GNU General Public License as published by
///   the Free Software Foundation, either version 3 of the License, or
///   (at your option) any later version.
///
///   This program is distributed in the hope that it will be useful,
///   but WITHOUT ANY WARRANTY; without even the implied warranty of
///   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///   GNU General Public License for more details.
///
///   You should have received a copy of the GNU General Public License
///   along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
//////////////////////////////////////////////////////////////////////////////

#include "petitionplay.h"

PetitionPlay::PetitionPlay(QObject *parent,QHostAddress address,int port,int id,QString subtitle)
    : CNContPetition(parent,QString("PLAY"),address,port)
{
    _id=id;
    _subtitle=subtitle;
}

void PetitionPlay::setMovie(int idMovie,QString subtitle)
{
    _id=idMovie;
    _subtitle=subtitle;
}

QByteArray PetitionPlay::buildJsonPetition()
{
    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);

    QJsonObject obJson;
    obJson.insert(QStringLiteral("command"),QStringLiteral("play"));
    obJson.insert(QStringLiteral("movie_id"),_id);
    obJson.insert(QStringLiteral("subtitles"),_subtitle);
    QJsonDocument p(obJson);
    out << (quint16)0 << QString(p.toJson(QJsonDocument::Compact));

    out.device()->seek(0);
    out << (quint16)(message.size()-sizeof(quint16));

    qDebug() << "ID MOVIE: "<< _id;
    return message;
}

void PetitionPlay::startPetition()
{
    _isSizeOfResponseReady=false;
    _bytesAvailable=0;

    QByteArray commandPlay=buildJsonPetition();

    _socket->write(commandPlay);
}

QJsonDocument PetitionPlay::getStatus()
{
    return doc;
}

void PetitionPlay::infoReady()
{
    _bytesAvailable=_socket->bytesAvailable();

    if(!_isSizeOfResponseReady) {
        if(_bytesAvailable < sizeof(quint16))
            return;

        *stream >> _sizeResponse;
        _isSizeOfResponseReady=true;
        _bytesAvailable-=sizeof(quint16);
        if(_bytesAvailable<=0)
            return;
    }

    if(_sizeResponse>0) {
        if(_bytesAvailable<_sizeResponse)
            return;

        QString aux;
        *stream >> aux;
        _sizeResponse=0;
        QByteArray response=QByteArray(aux.toStdString().c_str());
        doc=QJsonDocument::fromJson(response);
    }

    QByteArray lastMessage;
    QDataStream out(&lastMessage,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QStringLiteral("OK");
    out.device()->seek(0);
    out << (quint16)(lastMessage.size()-sizeof(quint16));
    _socket->write(lastMessage);
    _socket->waitForBytesWritten();
    _socket->disconnectFromHost();

    emit petitionReady();


}
