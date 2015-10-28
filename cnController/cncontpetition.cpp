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

#include "cncontpetition.h"

CNContPetition::CNContPetition(QObject *parent, QString type, QHostAddress address, int port) :
    QObject(parent),
    _type(type)
{
    _socket=new QTcpSocket(this);
    _address=address;
    _port=port;
    connect(_socket,SIGNAL(readyRead()),this,SLOT(infoReady()));
    connect(_socket,SIGNAL(connected()),this,SLOT(startPetition()));
}

void CNContPetition::start()
{
    stream=new QDataStream(_socket);
    stream->setVersion(QDataStream::Qt_5_0);

    _socket->connectToHost(_address,_port);
}

