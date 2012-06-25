#include "firstscreen.h"

FirstScreen::FirstScreen( PackageBackend *backend, QWidget *stageWidget, const QString& filename, QObject *parent )
{
    QWidget *warningWidget = new QWidget;
    QWidget *repoWidget = new QWidget;
    QWidget *packageWidget = new QWidget;
    QHBoxLayout *warningLayout = new QHBoxLayout( warningWidget );
    warningWidget->setStyleSheet( "background-color: rgb(255, 170, 127);" );
    repoWidget->setStyleSheet( "background-color : white" );
    packageWidget->setStyleSheet( "background-color : white" );
    VBoxLayout *installLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVBoxLayout *repoLayout = new QVBoxLayout( repoWidget );
    QVBoxLayout * packageLayout = new QVBoxLayout( packageWidget );
        
    //Create Interface Elemenets
    m_warning = new	QLabel( "This is a warning Message" );	//This should be done only if repositories to be added need to be trusted
    m_moreDetails = new QLabel( "<a href = \"#\">Click to view more details</a>" );
    m_trust = new QPushButton( "Trust" );		// Same as above
    m_trust->setStyleSheet( "QPushButton{ color : black; background-color : white; }" );
    //m_trust->setStyleSheet( "background-color : white" );
    m_settings = new QPushButton( "Settings" );
    m_cancel = new QPushButton( "Cancel" );
    m_install = new QPushButton( "Install" );
    m_install->setEnabled( false );

    //Add Elements to corresponding Layouts;
    warningLayout->addWidget( m_warning );
    warningLayout->addWidget( m_trust );
    warningLayout->addWidget( m_moreDetails );
    buttonLayout->addWidget( m_settings );
    buttonLayout->addSpacing( 100 );
    buttonLayout->addWidget( m_cancel );
    buttonLayout->addWidget( m_install );

    mainLayout->addWidget( warningWidget );
    mainLayout->addWidget( repoWidget );
    mainLayout->addWidget( packageWidget );
    mainLayout->addLayout( installLayout );
    mainLayout->addLayout( buttonLayout );

    //Signal Slot connections
    QObject::connect( m_settings, SIGNAL( clicked() ), this, SLOT( showSettings() ) );
    QObject::connect( m_install, SIGNAL( clicked() ), this, SLOT( performInstallation() ) );
    QObject::connect( m_cancel, SIGNAL(clicked()), parent, SLOT(deleteLater()) );
    QObject::connect( m_trust, SIGNAL(clicked()), this, SLOT(trust()) );
    QObject::connect( m_moreDetails, SIGNAL( linkActivated( QString) ), this, SLOT( untrusedRepoDetails(QString) ) );
    setLayout( mainLayout );
    show();

    m_stageWidget = this;

    //Parse YMP File
    m_backend = backend;
    OCI::YmpParser parser( filename );
    parser.parse();
    QList< OCI::Package* > packages = parser.packages();
    QList< OCI::Repository* > repos = parser.repositories();

    packageLayout->addWidget( new QLabel( "The following packages will be installed:" ) );
    foreach ( OCI::Package* iter, packages ) {
        m_backend->addPackage( iter->name() );
        QCheckBox *checkPackage = new QCheckBox( iter->name() );
        packageLayout->addWidget( checkPackage );
    }

    //Add Repository
    repoLayout->addWidget( new QLabel( "The following repositories will be added: " ) );
    foreach( OCI::Repository* iter, repos){
        m_backend->addRepository( QUrl( iter->url() ) );
        QCheckBox *checkRepo = new QCheckBox( iter->url() );
        repoLayout->addWidget( checkRepo );
    }
    m_detailsVisible = false;
}

void FirstScreen::showSettings()
{
    new Settings();
}

void FirstScreen::performInstallation()
{
    m_backend->install();
    m_stageWidget->hide();
    InstallScreen *installer = new InstallScreen( m_backend );
    m_stageWidget->parentWidget()->layout()->addWidget( installer );
    m_stageWidget = installer;
}

void FirstScreen::trust()
{
    /*
      Add code to trust repositories
    */
    m_install->setEnabled( true );
}

void FirstScreen::untrusedRepoDetails( QString link )
{
    if( !m_detailsVisible ){
        QLabel *gpgKeyName = new QLabel( "Name: " );
        QLabel *fingerprint = new QLabel( "Fingerprint: " );
        m_trust->parentWidget()->layout()->addWidget( gpgKeyName );
        m_trust->parentWidget()->layout()->addWidget( fingerprint );
        m_detailsVisible = true;
    }
}
