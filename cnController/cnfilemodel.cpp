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
#include "cnfilemodel.h"

CNFileModel::CNFileModel(QObject *parent, QList<CNFileEntry *> entry) : QAbstractTableModel(parent)
{
    _entryList=entry;
    generateRoleNames();
}

Qt::ItemFlags CNFileModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return QAbstractTableModel::flags(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int CNFileModel::rowCount(const QModelIndex &parent) const
{
    return _entryList.count();
}

int CNFileModel::columnCount(const QModelIndex &parent) const
{
    //Name, format(video type), hasSubtitles
    return 4;
}

void CNFileModel::generateRoleNames()
{
    if(!_roleNames.isEmpty())
        _roleNames.clear();

    _roleNames.insert(Qt::UserRole+1,QVariant("id").toByteArray());
    _roleNames.insert(Qt::UserRole+2,QVariant("name").toByteArray());
    _roleNames.insert(Qt::UserRole+3,QVariant("type").toByteArray());
    _roleNames.insert(Qt::UserRole+4,QVariant("hasSubtitles").toByteArray());
}

QHash<int,QByteArray> CNFileModel::roleNames() const
{
    return _roleNames;
}

QVariant CNFileModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if(!index.isValid())
        return res;

    //Default
    if(role==Qt::DisplayRole) {
        int column=index.column();
        switch(column) {
        case 0: ///id
            res=_entryList.at(index.row())->id();
            break;
        case 1: ///name
            res=_entryList.at(index.row())->name();
            break;
        case 2: ///type
            res=_entryList.at(index.row())->type();
            break;
        case 3: ///hasSubtitles
            res=_entryList.at(index.row())->hasSubtitles();
            break;
        default:
            break;
        }
        return res;
    }

    //User role (QtQuick)

    if(role==Qt::UserRole+1) res=_entryList.at(index.row())->id();
    if(role==Qt::UserRole+2) res=_entryList.at(index.row())->name();
    if(role==Qt::UserRole+3) res=_entryList.at(index.row())->type();
    if(role==Qt::UserRole+4) res=_entryList.at(index.row())->hasSubtitles();

    return res;
}

void CNFileModel::update(QList<CNFileEntry *> list)
{
    beginResetModel();
    clearEntryList();
    _entryList=list;
    endResetModel();
    qDebug() << "UPDATED MODEL";
}

void CNFileModel::clearEntryList()
{
    foreach(CNFileEntry *entry,_entryList)
        delete entry;

    _entryList.clear();
}

QStringList CNFileModel::getSubtitles(int id)
{
    QStringList res;
    foreach(CNFileEntry *entry,_entryList) {
        if(entry->id()==id)
            res=entry->getSubtitles();
    }
    return res;
}
