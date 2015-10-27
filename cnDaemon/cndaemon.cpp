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
#include "cndaemon.h"
#include "petitionattende.h"

CNDaemon::CNDaemon(QObject *parent, QString cPath) : QObject(parent)
{
    _configFile=cPath;
    _innerServer=0;
}

void CNDaemon::run()
{
    log(QStringLiteral("Init daemon"));
    if(_configFile.isEmpty() || _configFile.isNull()) {
        log(QStringLiteral("ERROR: No config file"));
        forcedStop(1);
        return;
    }
    configureDaemon();
    lookForMovieEntries();
    buildListOfFiles();
    prepareCastnow();
    configureAndStartTCP();
    startDiscoverService();
    return;
}

void CNDaemon::log(QString message)
{
    _log.write(message);
}

void CNDaemon::stop()
{
    log(QStringLiteral("Close daemon"));
    QCoreApplication::exit(0);
}

void CNDaemon::forcedStop(int num)
{
    log(QStringLiteral("Forced close daemon"));
    QCoreApplication::exit(num);
}

void CNDaemon::configureDaemon()
{
    QFileInfo info(_configFile);

    if(!info.exists()) {
        log(QStringLiteral("Building empty configuration file"));
        QJsonObject obJson;
        obJson.insert(QStringLiteral("interface"),"ALL");
        obJson.insert(QStringLiteral("port"),5000);
        obJson.insert(QStringLiteral("discovery_port"),5050);
        obJson.insert(QStringLiteral("server_name"),"CN Server");
        QJsonDocument doc(obJson);
        QFile outConf(info.absoluteFilePath());
        if(!outConf.open(QFile::Text | QFile::WriteOnly)) {
            log(QStringLiteral("ERROR: Can't open the conf file"));
            forcedStop(2);
            return;
        }
        outConf.write(doc.toJson());
        outConf.close();
    }

    //Read

    log(QStringLiteral("Reading configuration file (")+info.absoluteFilePath()+')');

    QFile inConf(info.absoluteFilePath());
    if(!inConf.open(QFile::Text | QFile::ReadOnly)) {
        log(QStringLiteral("ERROR: Can't read the conf file, file not open"));
        forcedStop(3);
        return;
    }
    QJsonDocument docJson=QJsonDocument::fromJson(inConf.readAll());
    inConf.close();

    if(!docJson.isObject()) {
        log(QStringLiteral("ERROR: Invalid configuration file"));
        forcedStop(4);
        return;
    }
    QJsonObject obJson=docJson.object();

    //Interface
    if(!obJson.contains(QStringLiteral("interface"))) {
        log(QStringLiteral("Interface not found, listen on ALL"));
        _listenAddress=QHostAddress::Any;
    } else {
        QString jsonInterface=obJson.value(QStringLiteral("interface")).toString().toUpper();
        if(jsonInterface==QStringLiteral("ALL")) {
            _listenAddress=QHostAddress::Any;
            log(QStringLiteral("Listening on ALL interfaces"));
        } else {
            QList<QNetworkInterface> listInterface=QNetworkInterface::allInterfaces();
            foreach(QNetworkInterface interface,listInterface) {
                if(interface.name().toUpper()==jsonInterface) {
                    _listenAddress=interface.addressEntries().first().ip();
                    log(QStringLiteral("Listening on ")+_listenAddress.toString());
                }
            }
        }

    }

    //myip
    if(obJson.contains(QStringLiteral("myip"))) {
        QString myipInterface=obJson.value(QStringLiteral("myip")).toString().toUpper();
        QList<QNetworkInterface> listInterface=QNetworkInterface::allInterfaces();
        foreach(QNetworkInterface interface,listInterface) {
            if(interface.name().toUpper()==myipInterface) {
                _myIp=interface.addressEntries().first().ip();
                log(QStringLiteral("My IP is ")+_myIp.toString());
            }
        }
    } else _myIp=QHostAddress();

    //Listening port
    if(!obJson.contains(QStringLiteral("port"))) {
        log(QStringLiteral("Port not found, listen on")+QString::number(5000));
        _port=5000;
    } else {
        _port=obJson.value(QStringLiteral("port")).toInt();
        log(QStringLiteral("Port: ")+QString::number(_port));
    }

    //Discover port
    if(!obJson.contains(QStringLiteral("discovery_port"))) {
        log(QStringLiteral("Discovery port not found, listen on ")+QString::number(5050));
        _discoverPort=5050;
    } else {
        _discoverPort=obJson.value(QStringLiteral("discovery_port")).toInt();
        log(QStringLiteral("Discovery port: ")+QString::number(_discoverPort));
    }

    //Server name
    if(!obJson.contains(QStringLiteral("server_name"))) {
        log(QStringLiteral("Server name not found, using 'CN server' as name"));
        _serverName=QStringLiteral("CN server");
    } else {
        _serverName=obJson.value(QStringLiteral("server_name")).toString();
        log(QStringLiteral("Server name: ")+_serverName);
    }

    //Dir path
    _dirPath.clear();
    if(!obJson.contains(QStringLiteral("directory"))) {
        log(QStringLiteral("Directory path not found, using '.'"));
        _dirPath.append(qApp->applicationDirPath());
    } else {
        QJsonArray dirInfo=obJson.value(QStringLiteral("directory")).toArray();
        QJsonArray::iterator it;
        for(it=dirInfo.begin();it!=dirInfo.end();it++) {
            _dirPath.append((*it).toString());
            log(QStringLiteral("Directory: ")+(*it).toString());
        }
    }
}

void CNDaemon::startDiscoverService()
{
    log(QStringLiteral("Starting discovery service"));
    _discoverSocket.bind(_listenAddress,_discoverPort);
    connect(&_discoverSocket,SIGNAL(readyRead()),this,SLOT(discoverPetition()));
    log(QStringLiteral("Discovery service ready"));
}

void CNDaemon::buildListOfFiles()
{
    log(QStringLiteral("Building JSON array of files"));
    QJsonArray arrayFiles;
    QList<CacheEntry*> entryList=_cache.values();
    foreach(CacheEntry* entry,entryList)
        arrayFiles.append(entry->toJson());
    _list_of_files=QJsonDocument(arrayFiles);
    log(QStringLiteral("JSON array is ready"));
}

void CNDaemon::discoverPetition()
{
    while(_discoverSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_discoverSocket.pendingDatagramSize());
        QHostAddress remitent;
        short unsigned int senderPort;
        _discoverSocket.readDatagram(datagram.data(),datagram.size(),&remitent,&senderPort);

        if(QString(datagram)=="CNDaemon_lookup") {
            QJsonObject responseObject;
            responseObject.insert("status","CNDaemon_found");
            responseObject.insert("server_name",_serverName);
            responseObject.insert("port",_port);
            QJsonDocument doc(responseObject);
            QByteArray correctResponse=doc.toJson(QJsonDocument::Compact);
            _discoverSocket.writeDatagram(correctResponse.toStdString().c_str(),correctResponse.size(),remitent,senderPort);
            log(QStringLiteral("Discover petition from ")+remitent.toString()+QStringLiteral(" ANSWERED"));
        } else {
            log(QStringLiteral("Discover petition from ")+remitent.toString()+QStringLiteral(" DISCARDED"));
        }
    }
}

CNProcess::Status CNDaemon::getStatus() const
{
    return _castnowProcess.currentStatus();
}

void CNDaemon::lookForMovieEntries()
{
    cleanCache();
    log(QStringLiteral("Building file cache"));
    CacheEntry *entry=0;
    int id=0;
    QFileInfoList subList;
    foreach(QString dir, _dirPath) {
        QDirIterator dirIt(QDir(dir),QDirIterator::Subdirectories);
        while(dirIt.hasNext()) {
            dirIt.next();
            //Discard non-movie formats
            QFileInfo info=dirIt.fileInfo();
            if(info.isFile() && info.isReadable()) {
                QString suffix=info.suffix().toUpper();
                if((suffix==QStringLiteral("MKV")) ||
                    (suffix==QStringLiteral("MP4")) ||
                        (suffix==QStringLiteral("AVI"))) {
                    entry=new CacheEntry(this,id,info);
                    _cache.insert(id++,entry);
                }
                if(suffix==QStringLiteral("SRT"))
                    subList.append(info);
            }
        }
    }

    //Merge subtitles list with movie list
    QList<CacheEntry*> cacheLine=_cache.values();
    foreach(QFileInfo subInfo,subList) {
        foreach(CacheEntry *entry,cacheLine)
            if(subInfo.fileName().remove(subInfo.suffix())==entry->name().remove(entry->name().count()-3,entry->name().count()+1)) {
                entry->addSubtitle(QPair<QString,QString>(subInfo.fileName(),subInfo.absoluteFilePath()));
            }
    }
    log(QStringLiteral("File cache ready (")+QString::number(id)+QStringLiteral(") files"));
}

void CNDaemon::configureAndStartTCP()
{
    if(!_innerServer) {
        log(QStringLiteral("Building TCP server"));
        _innerServer=new QTcpServer(this);
        connect(_innerServer,SIGNAL(newConnection()),this,SLOT(petitionToDaemon()));
        log(QStringLiteral("TCP server builded"));
    }
    if(_innerServer->isListening()) {
        log(QStringLiteral("Stopping TCP server"));
        _innerServer->close();
        log(QStringLiteral("TCP server stopped"));
    }
    log(QStringLiteral("Starting TCP server"));
    _innerServer->listen(_listenAddress,_port);
    log(QStringLiteral("Listening on ")+_listenAddress.toString()+':'+QString::number(_port));
}

void CNDaemon::petitionToDaemon()
{
    QTcpSocket *s=_innerServer->nextPendingConnection();
    PetitionAttende *petition=new PetitionAttende(this,s,this);
    connect(petition,SIGNAL(workDone()),this,SLOT(petitionDone()));
    connect(petition,SIGNAL(messageToDaemon(QString)),this,SLOT(log(QString)));
    _petitionList.append(petition);
}

void CNDaemon::petitionDone()
{
    PetitionAttende *petition=qobject_cast<PetitionAttende*>(sender());

    _petitionList.removeAll(petition);
    delete petition;
}

QJsonDocument CNDaemon::getListOfFiles() const
{
    return _list_of_files;
}

void CNDaemon::cleanCache()
{
    if(!_cache.isEmpty()) {
        QList<CacheEntry*> listEntry=_cache.values();
        foreach(CacheEntry *entry,listEntry)
            delete entry;
        _cache.clear();
    }
}

void CNDaemon::prepareCastnow()
{
    log(QStringLiteral("Preparing castnow"));
    _castnowProcess.setAddress(_myIp);
    connect(&_castnowProcess,SIGNAL(toLog(QString)),this,SLOT(log(QString)));
    log(QStringLiteral("Castnow is ready"));
}

void CNDaemon::tryToPlay(int id, QString subtitles)
{
    CacheEntry *entry=_cache.value(id);

    if(!entry)
        return;

    _castnowProcess.play(entry,subtitles);
}
