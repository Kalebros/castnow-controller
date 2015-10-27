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
#include "petitionattende.h"
#include "cndaemon.h"

PetitionAttende::PetitionAttende(QObject *parent, QTcpSocket *s, CNDaemon *d) : QObject(parent)
{
    _socket=s;
    _daemon=d;
    _bytesAvailable=0;
    _isCommandRead=false;
    _isSizeRead=false;
    _isCommandAttend=false;
    _commandSize=0;

    stream=new QDataStream(_socket);
    stream->setVersion(QDataStream::Qt_5_0);

    connect(_socket,SIGNAL(readyRead()),this,SLOT(informationReady()));
}

void PetitionAttende::informationReady()
{
    _bytesAvailable=_socket->bytesAvailable();

    if(!_isCommandRead) {
        if(!_isSizeRead) {
            if(_bytesAvailable<sizeof(qint16))
                return;

            *stream>> _commandSize;
            _isCommandRead=true;
        }
        if(_bytesAvailable<_commandSize)
            return;
        *stream >> _command;
        _jsonCommand=QJsonDocument::fromJson(_command.toUtf8());

        emit messageToDaemon(_jsonCommand.object().value(QStringLiteral("command")).toString().toUpper()
                             +QStringLiteral(" petition from ")+_socket->peerAddress().toString()+':'+QString::number(_socket->peerPort()));
        _isCommandRead=true;
    }
    if(!_isCommandAttend) {
        QByteArray response=attendCommand();
        _socket->write(response);
        _socket->waitForBytesWritten();
        emit messageToDaemon(whichCommand()+QStringLiteral(" from ")+_socket->peerAddress().toString()+':'+QString::number(_socket->peerPort())+QStringLiteral(" ANSWERED"));
        _isCommandAttend=true;
        _isSizeRead=false;
        _commandSize=0;
        return;
    }
    if(!_isSizeRead)
        if(_bytesAvailable<sizeof(quint16)) {
            return;
            *stream >> _commandSize;
    }
    QString confirmation;
    *stream >> confirmation;
    _socket->waitForDisconnected();
    emit workDone();
}

QString PetitionAttende::whichCommand() const
{
    return _jsonCommand.object().value(QStringLiteral("command")).toString().toUpper();
}

QByteArray PetitionAttende::attendCommand()
{
    QJsonObject command=_jsonCommand.object();
    QString instruction=command.value(QStringLiteral("command")).toString().toUpper();

    if(instruction==QStringLiteral("LIST"))
        return attendList();
    if(instruction==QStringLiteral("PLAY"))
        return attendPlay();

    return QByteArray();
}

QByteArray PetitionAttende::attendList()
{
    QByteArray response;
    QDataStream out(&response,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << (quint16)0 << QString(_daemon->getListOfFiles().toJson(QJsonDocument::Compact));
    out.device()->seek(0);
    out << (quint16)(response.size()-sizeof(quint16));
    return response;
}

QByteArray PetitionAttende::attendPlay()
{
    QJsonObject command=_jsonCommand.object();
    int idMovie=command.value(QStringLiteral("movie_id")).toInt();
    QString subtitle=command.value(QStringLiteral("subtitles")).toString();

    //STUB
    QJsonObject response;
    response.insert(QStringLiteral("status"),QString("playing"));
    response.insert(QStringLiteral("movie_id"),idMovie);
    response.insert(QStringLiteral("subtitle"),subtitle);
    QJsonDocument responseDoc(response);

    _daemon->tryToPlay(idMovie,subtitle);
    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out <<(quint16)0 << QString(responseDoc.toJson(QJsonDocument::Compact));
    out.device()->seek(0);
    out << (quint16)(response.size()-sizeof(quint16));

    return message;
}
