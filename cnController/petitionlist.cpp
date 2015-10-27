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
#include "petitionlist.h"

PetitionList::PetitionList(QObject *parent,QHostAddress address,int port) :
    CNContPetition(parent,QString("LIST"),address,port)
{
    _fileList.clear();
}

void PetitionList::startPetition()
{
    _isSizeOfListReady=false;
    _bytesAvailable=0;

    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);

    QJsonObject jsonPetition;
    jsonPetition.insert(QStringLiteral("command"),QStringLiteral("list"));
    QJsonDocument p(jsonPetition);
    out << (quint16)0 << QString(p.toJson(QJsonDocument::Compact));
    out.device()->seek(0);
    out << (quint16)(message.size()-sizeof(quint16));

    _socket->write(message);
}

void PetitionList::infoReady()
{
    _bytesAvailable=_socket->bytesAvailable();
    if(!_isSizeOfListReady) {
        if(_bytesAvailable<sizeof(quint16))
            return;

        *stream >> _sizeOfList;
        _socket->flush();
        _isSizeOfListReady=true;
        _bytesAvailable-=sizeof(quint16);
        if(_bytesAvailable<=0)
            return;
    }

    if(_sizeOfList>0) {
        if(_bytesAvailable<_sizeOfList)
            return;

        QString aux;
        *stream >> aux;
        _sizeOfList=0;
        _fileList=QByteArray(aux.toStdString().c_str());
        doc=QJsonDocument::fromJson(_fileList);
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
