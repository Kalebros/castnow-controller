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
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include <QStringList>

#include <QUdpSocket>
#include "cncontroller.h"
#include "cnfilemodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    CNController controlador;
    CNFileModel *fileModel=new CNFileModel(&app);

    QObject::connect(&controlador,SIGNAL(fileList(QList<CNFileEntry*>)),fileModel,SLOT(update(QList<CNFileEntry*>)));

    engine.rootContext()->setContextProperty(QStringLiteral("controller"),&controlador);
    engine.rootContext()->setContextProperty(QStringLiteral("fileModel"),fileModel);

    return app.exec();
}
