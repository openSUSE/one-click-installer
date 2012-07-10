#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QUrl>
#include "backend.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv, false );
    qDebug() << "Created new Application";
    QFile dataFile( argv[ 1 ] );

    if( !dataFile.open( QIODevice::ReadOnly ) ) {
        qDebug() << "Failed to open Data File";
    }

    QTextStream inData( &dataFile );

    Backend *ptr = new Backend;

    QString line;
    line = inData.readLine();
    if( line == "repositories" ) {
        do {
            line = inData.readLine();
            if( line != "packages") {
                //qDebug() << line;
                ptr->addRepository( QUrl( line ) );
            }
        } while( line !="packages" );
    }
    do {
        line = inData.readLine();
        if( !line.isNull() ) {
            //qDebug() << line;
            ptr->addPackage( line );
        }
    } while( !line.isNull() );

    ptr->install();

    return 0;
}
