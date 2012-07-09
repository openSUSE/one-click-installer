#include "firstscreen.h"

FirstScreen::FirstScreen( PackageBackend *backend, QWidget *stageWidget, const QString& filename, QObject *parent )
{
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

    //Initialise Files
    QFile repoFile( "repos" );
    QFile packageFile( "packages" );
    QTextStream outRepo( &repoFile );
    QTextStream outPackages( &packageFile );
//    outRepo.setVersion( QDataStream::Qt_4_8 );
//    outPackages.setVersion( QDataStream::Qt_4_8 );
    if( !repoFile.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        qDebug() << "Could not Write Repository data to file";
    }

    if( !packageFile.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        qDebug() << "Could not Write Package data to file";
    }
        
    //Create Interface Elemenets
    m_warning = new	QLabel( "<b>Be careful!</b> Some Sources are not currently known. Installing<br />software requires trusting these sources" );
    m_warning->setStyleSheet( "border : 1px solid rgb(196,181,147); background-color: rgb(253, 227, 187); border-radius : 5px" );
    m_warning->setContentsMargins( 10,10,10,10 );
    m_settings = new QPushButton( "Settings" );
    m_cancel = new QPushButton( "Cancel" );
    m_install = new QPushButton( "Install" );

    //Add Elements to corresponding Layouts;
    warningLayout->addWidget( m_warning );
    buttonLayout->addWidget( m_settings );
    buttonLayout->addSpacing( 100 );
    buttonLayout->addWidget( m_cancel );
    buttonLayout->addSpacing( 10 );
    buttonLayout->addWidget( m_install );
    setLayout( mainLayout );
    show();

    m_stageWidget = this;

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
        outRepo << iter->url();
        outRepo << "\n";
        m_backend->addRepository( QUrl( iter->url() ) );
        QHBoxLayout *sourceInfo = new QHBoxLayout;
        Details *detailsWidget = new Details( m_backend, m_repos.at( i )->url() );
        m_details.insert( i, detailsWidget );
        repoDetails = new QVBoxLayout;
        m_repoLayouts.append( repoDetails );
        m_visible.append( false );
        QLabel *repoName = new QLabel( "<b>Source:</b> " + iter->name() );
        QLabel *detailsLabel = new QLabel( QString("<a href = %1>Show Details</a>").arg( i ) );
        m_detailsLabels.insert( i, detailsLabel );
        repoName->setContentsMargins( 10,10,10,10 );
        repoName->setStyleSheet( "background-color: rgb(254, 250, 210); border-bottom : 1px solid rgb(252,233,79); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147);" );
        detailsLabel->setStyleSheet( "background-color: rgb(254, 250, 210); border-bottom : 1px solid rgb(252,233,79); border-right : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147);" );

        QObject::connect( detailsLabel, SIGNAL( linkActivated(QString) ), this, SLOT( showDetails( QString ) ) );

        sourceInfo->addWidget( repoName );
        sourceInfo->addWidget( detailsLabel );
        repoDetails->addLayout( sourceInfo );
        mainLayout->addLayout( repoDetails );

        QVBoxLayout *repoPackages = new QVBoxLayout;

        foreach( OCI::Package *iter, m_packages ) {
            outPackages << iter->name();
            outPackages << "\n";
            m_backend->addPackage( iter->name() );
            QCheckBox *checkPackage = new QCheckBox( iter->name() );
            checkPackage->setContentsMargins( 20,20,20,20 );
            checkPackage->setStyleSheet( "background-color : white; border-left : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); border-bottom : 1px solid rgb(196,181,147) ; padding-top : 10px; padding-bottom : 10px; padding-left : 3px;" );
            repoPackages->addWidget( checkPackage );
        }
        mainLayout->addLayout( repoPackages );
        i++;
    }

    //Signal Slot connections
    QObject::connect( m_settings, SIGNAL( clicked() ), this, SLOT( showSettings() ) );
    QObject::connect( m_install, SIGNAL( clicked() ), this, SLOT( performInstallation() ) );
    QObject::connect( m_cancel, SIGNAL( clicked()), parent, SLOT( close() ) );

    mainLayout->addLayout( installLayout );
    mainLayout->addWidget( warningWidget );
    mainLayout->addSpacing( 10 );
    mainLayout->addLayout( buttonLayout );

    repoFile.close();
    packageFile.close();
}

void FirstScreen::showSettings()
{
    new Settings();
}

void FirstScreen::performInstallation()
{
    //m_backend->install();
    m_stageWidget->hide();
    //InstallScreen *installer = new InstallScreen( m_backend );
    Summary *installSummary = new Summary( m_backend, m_stageWidget );
    m_stageWidget->parentWidget()->layout()->addWidget( installSummary );
    m_stageWidget = installSummary;
}

void FirstScreen::showDetails( QString link )
{
    int linkNo = link.toInt();

    if( m_visible.at( linkNo ) ) {
        m_detailsLabels[ linkNo ]->setText( QString( "<a href = %1>Show Details</a>" ).arg( linkNo ) );
        m_details[ linkNo ]->hide();
        m_visible.replace( linkNo, false );
    } else {
        m_detailsLabels[ linkNo ]->setText( QString( "<a href = %1>Hide Details</a>" ).arg( linkNo ) );
        m_repoLayouts.at( linkNo )->addWidget( m_details[ linkNo ] );
        m_details[ linkNo ]->show();
        m_visible.replace( linkNo, true );
    }
}

void FirstScreen::untrusedRepoDetails( QString link )
{
}
