#ifndef CNFILEMODEL_H
#define CNFILEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QHash>
#include <QString>
#include <QByteArray>
#include <QStringList>

#include <QList>
#include <QDebug>

#include "cnfileentry.h"

class CNFileModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CNFileModel(QObject *parent = 0,QList<CNFileEntry*> entry=QList<CNFileEntry*>());

    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QHash<int,QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent) const;
    Q_INVOKABLE int columnCount(const QModelIndex &parent) const;
    Q_INVOKABLE QStringList getSubtitles(int id);

signals:

public slots:

    void update(QList<CNFileEntry*> list);

private:

    void generateRoleNames();
    void clearEntryList();

    QList<CNFileEntry*> _entryList;
    QHash<int,QByteArray> _roleNames;
};


#endif // CNFILEMODEL_H
