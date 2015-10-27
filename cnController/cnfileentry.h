#ifndef CNFILEENTRY_H
#define CNFILEENTRY_H

#include <QObject>

class CNFileEntry : public QObject
{
    Q_OBJECT
public:
    explicit CNFileEntry(QObject *parent = 0,int id=-1,QString name=QString(),QString format=QString(),bool subtitles=false);

    int id() const
    { return _id; }

    QString name() const
    { return _name; }

    QString type() const
    { return _format; }

    bool hasSubtitles() const
    { return _hasSubtitles; }

    void addSubtitles(QString subtitle);

    QStringList getSubtitles() const
    { return _subtitles; }

signals:

public slots:

private:

    QString _name;
    QString _format;
    bool _hasSubtitles;
    int _id;
    QStringList _subtitles;
};

#endif // CNFILEENTRY_H
