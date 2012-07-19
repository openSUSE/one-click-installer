#include "firstscreen.h"

FirstScreen::FirstScreen( PackageBackend *backend, QString *tmpFileName, const QString& filename, QObject *parent )
{
    m_tmpFileName = tmpFileName;

    QWidget *warningWidget = new QWidget;
    QWidget *repoWidget = new QWidget;
    QWidget *packageWidget = new QWidget;
    QHBoxLayout *warningLayout = new QHBoxLayout( warningWidget );
    //warningWidget->setStyleSheet( "" );
    repoWidget->setStyleSheet( "background-color : white" );
    packageWidget->setStyleSheet( "background-color : white" );
    QVBoxLayout *installLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing( 0 );

    QFile dataFile( m_tmpFileName->toAscii() );
    if( !dataFile.open( QIODevice::Truncate | QIODevice::WriteOnly ) ) {
        qDebug() << "Could not open Data File";
    }

    m_settings.sync();

    QTextStream outData( &dataFile );

    //Create Interface Elemenets
    m_warning = new	QLabel( "<b>Be careful!</b> Some Sources are not currently known. Installing<br />software requires trusting these sources" );
    m_warning->setStyleSheet( "border : 1px solid rgb(196,181,147); background-color: rgb(253, 227, 187); border-radius : 5px" );
    m_warning->setContentsMargins( 10,10,10,10 );
    m_showSettings = new QPushButton( "Settings" );
    m_cancel = new QPushButton( "Cancel" );
    m_install = new QPushButton( "Install" );

    //Add Elements to corresponding Layouts;
    warningLayout->addWidget( m_warning );
    buttonLayout->addWidget( m_showSettings );
    buttonLayout->addSpacing( 100 );
    buttonLayout->addWidget( m_cancel );
    buttonLayout->addSpacing( 10 );
    buttonLayout->addWidget( m_install );
    setLayout( mainLayout );

    //Parse YMP File
    m_backend = backend;
    OCI::YmpParser parser( filename );
    parser.parse();
    m_packages = parser.packages();
    m_repos = parser.repositories();

    //Add Repository
    int i = 0;
    QVBoxLayout *repoDetails;

    foreach( OCI::Repository *iter, m_repos) {
        m_backend->addRepository( QUrl( iter->url() ) );
        Details *detailsWidget = new Details( m_backend, i, m_repos.at( i ) );
        mainLayout->addWidget( detailsWidget );

        static int j = 0;
        foreach( OCI::Package *iter, m_packages ) {
            m_backend->addPackage( iter->name() );
            PackageDetails *packDetails = new PackageDetails( iter, j );
            mainLayout->addWidget( packDetails );
            j++;
        }
        i++;
    }

    foreach( QUrl iter, m_backend->repositories()) {
        outData << "R " << iter.toString() << "\n";
    }

    foreach( QString iter, m_backend->packages() ) {
        outData << "P " << iter << "\n";
    }

    //Signal Slot connections
    QObject::connect( m_showSettings, SIGNAL( clicked() ), this, SLOT( showSettings() ) );
    QObject::connect( m_install, SIGNAL( clicked() ), this, SLOT( performInstallation() ) );
    QObject::connect( m_cancel, SIGNAL( clicked()), parent, SLOT( close() ) );

    mainLayout->addLayout( installLayout );
    mainLayout->addWidget( warningWidget );
    mainLayout->addSpacing( 10 );
    mainLayout->addLayout( buttonLayout );

    //Show the widgets if the setting for showing the details is set
//    if( m_settings.value( "showdetails", 1 ).toInt() == 1 ) {
//        for( int i = 0; i < m_repos.count(); i++ ) {
//            showDetails( QString( "%1" ).arg( i ) );
//        }
//    }

    dataFile.close();
}

void FirstScreen::showSettings()
{
    new Settings( &m_settings );
}

void FirstScreen::performInstallation()
{
    if( m_settings.value( "proposal", 1 ).toInt() == 1 ) {
        emit showNextScreen( 1 );
    } else {
        emit showNextScreen( 2 );
    }
}

void FirstScreen::showEvent( QShowEvent *s )
{
    emit countChanged( m_repos.count(), m_packages.count() );
}

void FirstScreen::untrusedRepoDetails( QString link )
{
}
