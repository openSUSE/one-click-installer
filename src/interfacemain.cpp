//      interfacemain.cpp
//      
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
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QUrl>
#include <QStringList>
#include "backend.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv, false );
    qDebug() << "Created new Application";
    QFile dataFile( argv[ 1 ] );

    if( !dataFile.open( QIODevice::ReadOnly ) ) {
        qDebug() << "Failed to open Data File";
        return 1;
    }

    QTextStream inData( &dataFile );

    Backend *ptr = new Backend;

    QString line;
    do {
        line = inData.readLine();
        if( !line.isNull() ) {
            if( line.at( 0 ) == 'R' ) {
                QString repo = line.split( " " ).at( 1 );
                qDebug() << repo;
                ptr->addRepository( repo );
            } else if( line.at( 0 ) == 'P' ) {
                QString package = line.split( " " ).at( 1 );
                qDebug() << package;
                ptr->addPackage( package );
            }
        }
    } while( !line.isNull() );

//    ptr->install();

    return ptr->errorCode();
}
