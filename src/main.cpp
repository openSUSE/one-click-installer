#include <QApplication>
#include "mainwindow.h"

int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );
	MainWindow *win = new MainWindow( QString( argv[1] ) );
	win->show();
	return app.exec();
}
