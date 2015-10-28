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

#include "cncontroller.h"

CNController::CNController(QObject *parent) : QObject(parent)
{
    discoverSocket.bind(QHostAddress::Any,5050);
    connect(&discoverSocket,SIGNAL(readyRead()),this,SLOT(discoverResponse()));
    serverName=QString();
    _currentPetition=0;
}

void CNController::searchServer()
{
    QByteArray searchDatagram(QStringLiteral("CNDaemon_lookup").toStdString().c_str());
    discoverSocket.writeDatagram(searchDatagram,QHostAddress::Broadcast,5050);
}

void CNController::discoverResponse()
{
    while(discoverSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(discoverSocket.pendingDatagramSize());
        QHostAddress remitent;
        unsigned short int port;

        discoverSocket.readDatagram(datagram.data(),datagram.size(),&remitent,&port);
        QJsonDocument doc=QJsonDocument::fromJson(datagram);
        if(!doc.isEmpty() && doc.isObject()) {
            QJsonObject obj=doc.object();
            serverName=obj.value("server_name").toString();
            this->port=obj.value("port").toInt();
            address=remitent;
            emit serverFound(serverName,this->port);
            askForFiles();
        }
    }
}


void CNController::askForFiles()
{
    if(_currentPetition) {
        delete _currentPetition;
    }
    _currentPetition=new PetitionList(this,address,port);
    connect(_currentPetition,SIGNAL(petitionReady()),this,SLOT(currentPetitionEnded()));
    _currentPetition->start();
}

void CNController::currentPetitionEnded()
{
    this->disconnect(_currentPetition);
    if(_currentPetition->type()==QString("LIST")) {
        PetitionList *pList=qobject_cast<PetitionList*>(_currentPetition);
        QJsonArray arrayJson=pList->getList().array();
        QJsonArray::iterator it;
        QList<CNFileEntry*> entryList;
        CNFileEntry *entry;
        QJsonArray subArray;
        for(it=arrayJson.begin();it!=arrayJson.end();it++) {
            entry=new CNFileEntry(0,(*it).toObject().value(QStringLiteral("id")).toInt(),
                                  (*it).toObject().value(QStringLiteral("name")).toString(),
                                  (*it).toObject().value(QStringLiteral("type")).toString(),
                                  (*it).toObject().value(QStringLiteral("hasSubtitles")).toBool());
            subArray=(*it).toObject().value(QStringLiteral("subtitles")).toArray();
            QJsonArray::iterator aux;
            for(aux=subArray.begin();aux!=subArray.end();aux++)
                entry->addSubtitles((*aux).toString());
            entryList.append(entry);
        }
        emit fileList(entryList);
    }
    if(_currentPetition->type()==QStringLiteral("PLAY")) {
        PetitionPlay *pPlay=qobject_cast<PetitionPlay*>(_currentPetition);
        QJsonDocument result=pPlay->getStatus();
        QJsonObject obJson=result.object();
        qDebug() << obJson.value(QStringLiteral("status")).toString();
    }
    delete _currentPetition;
    _currentPetition=0;
}

void CNController::playFile(int id, QString subtitle)
{
    if(_currentPetition) {
        delete _currentPetition;
    }
    PetitionPlay *pPlay=new PetitionPlay(this,address,port,id,subtitle);
    _currentPetition=pPlay;
    connect(_currentPetition,SIGNAL(petitionReady()),this,SLOT(currentPetitionEnded()));
    _currentPetition->start();
}
