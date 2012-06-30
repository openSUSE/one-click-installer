#include <QApplication>
#include <QDebug>
#include "mainwindow.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    bool fakeRequested = false;
    if( argc == 1){
        qDebug() << "No Valid File Passed";
        return 0;
    }
    if( !QString( "--fake" ).compare( argv[ 2 ] ) ){
        qDebug()<< "FakeBackend being used";
        fakeRequested = true;
    }
    MainWindow *win = new MainWindow( QString( argv[1] ), fakeRequested );
    win->show();
    return app.exec();
}
