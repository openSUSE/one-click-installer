#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QUrl>
#include "backend.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    qDebug() << "Created new Application";
    QFile repoFile( "repos" );
    QFile packagesFile( "packages" );

    if( !repoFile.open( QIODevice::ReadOnly ) ) {
        qDebug() << "Could not Read Repos File";
    }

    if( !packagesFile.open( QIODevice::ReadOnly ) ) {
        qDebug() << "Could not Read Packages File";
    }

    QTextStream inRepos( &repoFile );
    QTextStream inPackages( &packagesFile );

    Backend *ptr = new Backend;

    QString line;
    do {
        line = inRepos.readLine();
        if( !line.isNull() ) {
            qDebug() << line;
            ptr->addRepository( QUrl( line ) );
        }
    } while( !line.isNull() );

    do {
        line = inPackages.readLine();
        if( !line.isNull() ) {
            qDebug() << line;
            ptr->addPackage( line );
        }
    } while( !line.isNull() );

    ptr->install();

    return 0;
}
