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
    QVBoxLayout *installLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing( 0 );
        
    //Create Interface Elemenets
    m_warning = new	QLabel( "This is a warning Message" );	//This should be done only if repositories to be added need to be trusted
    m_settings = new QPushButton( "Settings" );
    m_cancel = new QPushButton( "Cancel" );
    m_install = new QPushButton( "Install" );

    //Add Elements to corresponding Layouts;
    warningLayout->addWidget( m_warning );
    buttonLayout->addWidget( m_settings );
    buttonLayout->addSpacing( 100 );
    buttonLayout->addWidget( m_cancel );
    buttonLayout->addWidget( m_install );

    //Signal Slot connections
    QObject::connect( m_settings, SIGNAL( clicked() ), this, SLOT( showSettings() ) );
    QObject::connect( m_install, SIGNAL( clicked() ), this, SLOT( performInstallation() ) );
    QObject::connect( m_cancel, SIGNAL(clicked()), parent, SLOT(deleteLater()) );
    setLayout( mainLayout );
    show();

    m_stageWidget = this;

    //Parse YMP File
    m_backend = backend;
    OCI::YmpParser parser( filename );
    parser.parse();
    QList< OCI::Package* > packages = parser.packages();
    QList< OCI::Repository* > repos = parser.repositories();

    //Add Repository
    foreach( OCI::Repository *iter, repos){
        m_backend->addRepository( QUrl( iter->url() ) );
        QHBoxLayout *sourceInfo = new QHBoxLayout;
        QLabel *repoName = new QLabel( "<b>Source:</b> " + iter->name() );
        QLabel *detailsLabel = new QLabel( "<a href = \"#\">Show Details</a>" );
        repoName->setStyleSheet( "background-color : yellow" );
        detailsLabel->setStyleSheet( "background-color : yellow" );
        sourceInfo->addWidget( repoName );
        sourceInfo->addWidget( detailsLabel );
        mainLayout->addLayout( sourceInfo );
        QVBoxLayout *repoPackages = new QVBoxLayout;
        foreach( OCI::Package *iter, packages ){
            m_backend->addPackage( iter->name() );
            QCheckBox *checkPackage = new QCheckBox( iter->name() );
            checkPackage->setStyleSheet( "background-color : white ");
            repoPackages->addWidget( checkPackage );
        }
        mainLayout->addLayout( repoPackages );
    }

    mainLayout->addLayout( installLayout );
    mainLayout->addWidget( warningWidget );
    mainLayout->addLayout( buttonLayout );
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

void FirstScreen::untrusedRepoDetails( QString link )
{
}
