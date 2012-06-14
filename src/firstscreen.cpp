#include "firstscreen.h"

FirstScreen::FirstScreen( PackageBackend *backend, const QString& filename, QObject *parent )
{
	//Create Layouts
	QVBoxLayout *warningLayout = new QVBoxLayout;
	QVBoxLayout *installLayout = new QVBoxLayout;
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	QVBoxLayout *mainLayout = new QVBoxLayout;
        
        //Create Interface Elemenets
	m_warning = new	QLabel( "This is a warning Message" );	//This should be done only if repositories to be added need to be trusted
	m_trust = new QPushButton( "Trust" );		// Same as above

	
	m_settings = new QPushButton( "Settings" );
	m_cancel = new QPushButton( "Cancel" );
	m_install = new QPushButton( "Install" );

	//Add Elements to corresponding Layouts;
	warningLayout->addWidget( m_warning );
	warningLayout->addWidget( m_trust );
	buttonLayout->addWidget( m_settings );
	buttonLayout->addWidget( m_cancel );
	buttonLayout->addWidget( m_install );

	mainLayout->addLayout( warningLayout );
	mainLayout->addLayout( installLayout );
	mainLayout->addLayout( buttonLayout );

	//Signal Slot connections
	QObject::connect( m_settings, SIGNAL( clicked() ), this, SLOT( showSettings() ) );
	QObject::connect( m_install, SIGNAL( clicked() ), this, SLOT( performInstallation() ) );
	setLayout( mainLayout );
        show();

        //Parse YMP File
	m_backend = backend;
        OCI::YmpParser parser( filename );
        parser.parse();
        QList< OCI::Package* > packages = parser.packages();
        QList< OCI::Repository* > repos = parser.repositories();

	foreach ( OCI::Package* iter, packages ) {
		m_backend->performInstallation( iter->name() );
	}

	//Add Repository
	foreach( OCI::Repository* iter, repos){
        	m_backend->setToAddRepository( QUrl( iter->url() ) );
	}
	static_cast< FakeBackend* >( m_backend )->addRepositories();
}

void
FirstScreen::showSettings()
{
	new Settings();
}

void
FirstScreen::performInstallation()
{
	static_cast< FakeBackend* >( m_backend )->installPackages();
}
