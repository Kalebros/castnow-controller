#ifndef CACHEENTRY_H
#define CACHEENTRY_H

#include <QObject>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QPair>
#include <QMap>

#include <QDebug>

class CacheEntry : public QObject
{
    Q_OBJECT
public:
    explicit CacheEntry(QObject *parent = 0,int id=-1,QFileInfo info=QFileInfo(),bool subtitles=false);

    int id() const
    { return _id; }

    QString name() const
    { return _name; }

    QString format() const
    { return _format; }

    bool hasSubtitles() const
    { return _hasSubtitles; }

    QJsonObject toJson() const;

    void addSubtitle(QPair<QString,QString> subtitle);

    QString getSubtitle(QString subtitle);

    QFileInfo getInfo() const
    { return _info; }

signals:

public slots:

private:

    int _id;
    QString _name;
    QString _format;
    bool _hasSubtitles;
    QFileInfo _info;
    QMap<QString,QString> _subtitles;
};

#endif // CACHEENTRY_H
