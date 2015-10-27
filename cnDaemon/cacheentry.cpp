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
#include "cacheentry.h"

CacheEntry::CacheEntry(QObject *parent, int id, QFileInfo info, bool subtitles) : QObject(parent)
{
    _id=id;
    _name=info.fileName();
    _format=info.suffix().toUpper();
    _hasSubtitles=subtitles;
    _subtitles.clear();
    _info=info;
}

QJsonObject CacheEntry::toJson() const
{
    QJsonObject res;
    QJsonArray subArray;
    res.insert(QStringLiteral("id"),_id);
    res.insert(QStringLiteral("name"),_name);
    res.insert(QStringLiteral("type"),_format);
    res.insert(QStringLiteral("hasSubtitles"),_hasSubtitles);

    QStringList subKeys=_subtitles.keys();
    foreach(QString key,subKeys)
        subArray.append(key);

    res.insert(QStringLiteral("subtitles"),subArray);

    return res;
}

void CacheEntry::addSubtitle(QPair<QString, QString> subtitle)
{
    _subtitles.insert(subtitle.first,subtitle.second);
}

QString CacheEntry::getSubtitle(QString subtitle)
{
    qDebug() << subtitle << _subtitles.value(subtitle,QString());
    return _subtitles.value(subtitle,QString());
}

