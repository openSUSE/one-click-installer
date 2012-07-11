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
                qDebug() << line.split( " " ).at( 1 );
            } else if( line.at( 0 ) == 'P' ) {
                qDebug() << line.split( " " ).at( 1 );
            }
        }
    } while( !line.isNull() );

//    ptr->install();

    return ptr->errorCode();
}
