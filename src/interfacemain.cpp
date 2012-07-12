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
