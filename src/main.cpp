#include <QApplication>
#include <QDebug>
#include "mainwindow.h"

int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );
	if( argc == 1){
		qDebug() << "No Valid File Passed";
		return 0;
	}
	MainWindow *win = new MainWindow( QString( argv[1] ) );
	win->show();
	return app.exec();
}
