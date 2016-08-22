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
#include <klocalizedstring.h>
#include "mainwindow.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    bool fakeRequested = false;
    
    //Connecting to Catalogs
    KLocalizedString::setApplicationDomain("oneclickinstaller");
    
    QStringList args = app.arguments();
    if (args.contains("--fake")) {
        qDebug()<< "FakeBackend being used";
        fakeRequested = true;
        args.removeAll("--fake");
    }
    
    if( args.isEmpty() || !QFile::exists(args[0])) {
         qFatal( "No Valid File Passed" );
        return 0;
    }
    
    QString fileName = args[1];
    QString tmpFileName = QString( "/tmp/%1-%2" ).arg( QFileInfo( fileName ).fileName().split( "." ).at( 0 ) )
                                                 .arg( QUuid::createUuid().toString().split( "-" ).at( 1 ) );

    qDebug() << tmpFileName;
    MainWindow *win = new MainWindow( fileName, tmpFileName, fakeRequested );
    win->show();
    return app.exec();
}
