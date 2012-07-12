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
