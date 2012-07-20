#include "firstscreen.h"

FirstScreen::FirstScreen( PackageBackend *backend, QString *tmpFileName, const QString& filename, QObject *parent )
{
    m_tmpFileName = tmpFileName;

    setStyleSheet( "background-color : rgb(251,248,241)" );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing( 0 );

    QFile dataFile( m_tmpFileName->toAscii() );
    if( !dataFile.open( QIODevice::Truncate | QIODevice::WriteOnly ) ) {
        qDebug() << "Could not open Data File";
    }

    QTextStream outData( &dataFile );

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
            mainLayout->addSpacing( -10 );
            PackageDetails *packDetails = new PackageDetails( iter, j );
            mainLayout->addWidget( packDetails );
            j++;
        }
        i++;
        mainLayout->addSpacing( -10 );
    }

    foreach( QUrl iter, m_backend->repositories()) {
        outData << "R " << iter.toString() << "\n";
    }

    foreach( QString iter, m_backend->packages() ) {
        outData << "P " << iter << "\n";
    }

    dataFile.close();
}

void FirstScreen::showEvent( QShowEvent *s )
{
    emit countChanged( m_repos.count(), m_packages.count() );
}

void FirstScreen::untrusedRepoDetails( QString link )
{
}
