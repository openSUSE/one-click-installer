#ifndef MAINWINDOW_H
#define MAINWINDIW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QList>
#include "firstscreen.h"
#include "FakeBackend.h"
#include "PackageBackend.h"

class MainWindow : public QDialog
{
	public:
		MainWindow( QObject *parent = 0 );
	private:
		QLabel *m_info;
		QWidget *m_stageWidget;	//This will load the corresponding widget to the layout depending on the stage of the installation
                PackageBackend *backend;
};

#endif
