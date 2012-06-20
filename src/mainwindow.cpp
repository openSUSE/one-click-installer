#include "mainwindow.h"

MainWindow::MainWindow( const QString& filename, QObject *parent)
{
	m_info = new QLabel( "This installer will install and download packages" );
	QVBoxLayout *mainLayout = new QVBoxLayout;

        //Set up the backend
        m_backend = new FakeBackend( this );

        m_stageWidget = new FirstScreen( m_backend, filename );
	mainLayout->addWidget( m_info );
	mainLayout->addWidget( m_stageWidget );
	setLayout( mainLayout );
	setWindowTitle( "One Click Install" );

	show();
}
