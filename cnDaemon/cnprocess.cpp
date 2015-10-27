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
#include "cnprocess.h"

CNProcess::CNProcess(QObject *parent) : QObject(parent)
{
    _status=CNProcess::Idle;
    _currentEntry=0;
    _currentSubtitle=QString();

    _cnStream=new QTextStream(&_castnow);

    connect(&_castnow,SIGNAL(error(QProcess::ProcessError)),SLOT(errorProcess(QProcess::ProcessError)));

    _castnow.setProgram("castnow");
}

CNProcess::Status CNProcess::currentStatus() const
{
    return _status;
}

QJsonObject CNProcess::currentStatusJson() const
{
    QJsonObject res;

    ///@todo Implement
    return res;
}

void CNProcess::play(CacheEntry *entry,QString subtitle)
{
    if(_castnow.state()==QProcess::Running) {
        *_cnStream << "q";
        _castnow.waitForFinished();
    }

    _currentEntry=entry;
    _currentSubtitle=subtitle;

    QString cSubtitle=_currentEntry->getSubtitle(_currentSubtitle);

    emit toLog(QStringLiteral("Playing")+_currentEntry->name());

    QStringList arguments;
    if(!_address.isNull())
        arguments << "--myip" << _address.toString();
    qDebug() << "SUBTITLE: " << _currentSubtitle;
    if(!cSubtitle.isEmpty())
        arguments << "--subtitles" << cSubtitle;
    arguments << _currentEntry->getInfo().absoluteFilePath();

    //TESTING
    _castnow.setArguments(arguments);
    _castnow.start();
    _castnow.waitForStarted();
    _status=Playing;
    emit statusChanged(_status);
}

void CNProcess::stop()
{

}

void CNProcess::pause()
{

}

void CNProcess::errorProcess(QProcess::ProcessError error)
{
    qDebug() << "ERROR en proceso: " << error;
    _status=Idle;
    emit statusChanged(_status);
}




