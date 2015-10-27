#ifndef CNDAEMON_H
#define CNDAEMON_H

#include <QObject>
#include <QCoreApplication>
#include <QTextStream>
#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QDirIterator>
#include <QMap>
#include <QTcpServer>
#include <QProcess>

#include "logger.h"
#include "cacheentry.h"
#include "cnprocess.h"

class PetitionAttende;

class CNDaemon : public QObject
{
    Q_OBJECT
public:

    explicit CNDaemon(QObject *parent = 0,QString cPath=QString());

    CNProcess::Status getStatus() const;

public slots:

    /*!
     * Start the Castnow daemon
     */
    void run();
    QJsonDocument getListOfFiles() const;

    void tryToPlay(int id,QString subtitles);

private:

    Logger _log;      ///< Log stream
    QString _configFile;    ///< Config file path
    QUdpSocket _discoverSocket;
    int _port;
    int _discoverPort;
    QString _serverName;
    QStringList _dirPath;       ///< Directory with movies
    QHostAddress _listenAddress;
    QHostAddress _myIp;
    QMap<int,CacheEntry*> _cache;
    QJsonDocument _list_of_files;
    CNProcess _castnowProcess;
    QTcpServer *_innerServer;
    QList<PetitionAttende*> _petitionList;

private slots:
    /*!
     * Write a log entry
     * \param message   New entry
     */
    void log(QString message);

private:

    /*!
     * Stop the daemon
     */
    void stop();

    /*!
     * Force stop the daemon (i.e, a error crash)
     * \param num   Exit num
     */
    void forcedStop(int num);

    /*!
     * Read the configuration file and configure the daemon
     */
    void configureDaemon();

    void startDiscoverService();
    void lookForMovieEntries();
    void buildListOfFiles();
    void configureAndStartTCP();
    void prepareCastnow();
    void cleanCache();

protected slots:

    void discoverPetition();

    void petitionToDaemon();

    void petitionDone();
};

#endif // CNDAEMON_H
