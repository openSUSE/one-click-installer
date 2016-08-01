//      Copyright 2012 Saurabh Sood <saurabh@saurabh.geeko>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//      

#include <klocalizedstring.h>
#include "firstscreen.h"
#include "utils.h"

FirstScreen::FirstScreen( PackageBackend *backend, const QString& tmpFileName, const QString& filename, QObject *parent )
{
    m_tmpFileName = tmpFileName;
    setStyleSheet( "background-color : white;" );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing( 0 );
    setLayout( mainLayout );

    QFile dataFile( m_tmpFileName.toLocal8Bit() );
    if( !dataFile.open( QIODevice::Truncate | QIODevice::WriteOnly ) ) {
        qDebug() << "Could not open Data File";
    }
    QTextStream outData( &dataFile );

    //Parse YMP File
    m_backend = backend;
    OCI::YmpParser parser( filename );
    parser.parse();
    m_packages = parser.packages();
    m_repos = parser.repositories();

    //Add Repository
    int i = 0;
    m_numOfRepositories = 0;
    m_numOfPackages = 0;
    m_untrustedSources = 0;
    unsigned int packageID = 0;
    QVBoxLayout *repoDetails;
    foreach( OCI::Repository *repo, m_repos) {
        // Only proceed if it is the recommended repository
        if(repo->recommended() == "false")
            continue;
        ZypperUtils::initRepository(repo->url().toStdString());
        ++m_numOfRepositories;
        m_backend->addRepository( QUrl( repo->url() ) );
        
        RepositoryWidget *repositoryDetails = new RepositoryWidget( m_backend, i, m_repos.at( i ) );
        mainLayout->addWidget( repositoryDetails );
        if( !m_backend->exists( repo->url() ) )
            m_untrustedSources++;
        
        foreach( OCI::Package *package, m_packages ) {
            ++m_numOfPackages;
            m_backend->addPackage( package->name() );
            mainLayout->addSpacing( -10 );
            
            PackageDetails *packDetails = new PackageDetails( package, packageID++, m_packages.count() );
            QObject::connect( packDetails, SIGNAL( sizeUpdated( QString ) ), this, SIGNAL( sizeUpdated( QString ) ) );
            QObject::connect( packDetails, SIGNAL( installableStateToggled( bool ) ), this, SLOT( checkPackagesInstallableState() ) );
            mainLayout->addWidget( packDetails );
        }
        i++;
        mainLayout->addSpacing( -8 );
    }

    if( m_untrustedSources > 0 ) {
        m_warning = new	QLabel( i18n("<b>Be careful!</b> Some Sources are not currently known. Installing<br />software requires trusting these sources") );
        m_warning->setStyleSheet( "border : 1px solid rgb(196,181,147); background-color: rgb(253, 227, 187); border-radius : 10px" );
        m_warning->setContentsMargins( 10,10,10,10 );
        mainLayout->addSpacing( 20 );
        mainLayout->addWidget( m_warning );
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
    emit countChanged( m_numOfRepositories, m_numOfPackages );
    qDebug() << "number of untrusted sources is " << m_untrustedSources;
}

void FirstScreen::checkPackagesInstallableState()
{
    QLayout *layout = QWidget::layout();
    for (int i = 0; i < layout->count(); ++i) {
        PackageDetails *package = qobject_cast<PackageDetails *>(layout->itemAt(i)->widget());
        if (!package)    continue;
        if (package->shouldBeInstalled()) {
            emit packageListInstallableStateToggled( true );
            return;
        }
    }
    //for no packages
    emit packageListInstallableStateToggled( false );
}
