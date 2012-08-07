//      Copyright 2012 Saurabh Sood <saurabh@saurabh.geeko>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//      


#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QUuid>
#include <QFileInfo>
#include "mainwindow.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    bool fakeRequested = false;
    if( argc == 1) {
        qDebug() << "No Valid File Passed";
        return 0;
    }
    if( !QString( "--fake" ).compare( argv[ 2 ] ) ) {
        qDebug()<< "FakeBackend being used";
        fakeRequested = true;
    }
    QString fileName( "/tmp/" );
    fileName.append( QFileInfo( argv[1] ).fileName().split( "." ).at( 0 ) ).append( "-" );
    fileName.append( QUuid::createUuid().toString().split( "-" ).at( 1 ) );
    qDebug() << fileName;
    MainWindow *win = new MainWindow( QString( argv[1] ), fileName, fakeRequested );
    win->show();
    return app.exec();
}
