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
    int i = 0;
    QVBoxLayout *repoDetails;
    foreach( OCI::Repository *iter, repos){
        m_backend->addRepository( QUrl( iter->url() ) );
        QHBoxLayout *sourceInfo = new QHBoxLayout;
        repoDetails = new QVBoxLayout;
        m_repoLayouts.append( repoDetails );
        m_visible.append( false );
        QLabel *repoName = new QLabel( "<b>Source:</b> " + iter->name() );
        QLabel *detailsLabel = new QLabel( QString("<a href = %1>Show Details</a>").arg( i ) );
        m_detailsLabels.replace( i, detailsLabel );
        repoName->setStyleSheet( "background-color: rgb(255, 221, 139);" );
        detailsLabel->setStyleSheet( "background-color: rgb(255, 221, 139);" );
        QObject::connect( detailsLabel, SIGNAL( linkActivated(QString) ), this, SLOT( showDetails( QString ) ) );
        sourceInfo->addWidget( repoName );
        sourceInfo->addWidget( detailsLabel );
        repoDetails->addLayout( sourceInfo );
        mainLayout->addLayout( repoDetails );
        QVBoxLayout *repoPackages = new QVBoxLayout;
        foreach( OCI::Package *iter, packages ){
            m_backend->addPackage( iter->name() );
            QCheckBox *checkPackage = new QCheckBox( iter->name() );
            checkPackage->setStyleSheet( "background-color : white ");
            repoPackages->addWidget( checkPackage );
        }
        mainLayout->addLayout( repoPackages );
        i++;
    }

    //Signal Slot connections
    QObject::connect( m_settings, SIGNAL( clicked() ), this, SLOT( showSettings() ) );
    QObject::connect( m_install, SIGNAL( clicked() ), this, SLOT( performInstallation() ) );
    QObject::connect( m_cancel, SIGNAL(clicked()), parent, SLOT(deleteLater()) );

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
    //InstallScreen *installer = new InstallScreen( m_backend );
    Summary *installSummary = new Summary( m_backend, m_stageWidget );
    m_stageWidget->parentWidget()->layout()->addWidget( installSummary );
    m_stageWidget = installSummary;
}

void FirstScreen::showDetails( QString link )
{
    int linkNo = link.toInt();
    if( m_visible.at( linkNo ) ){
        Details *temp = m_detailsWidgets.at( linkNo );
        m_detailsWidgets.removeAt( linkNo );
        delete temp;
        m_visible.replace( linkNo, false);
        m_detailsLabels.at( linkNo )->setText( QString( "<a href = %1>Show Details</a>" ).arg( linkNo ) );
    } else{
        qDebug() << linkNo;
        Details *detailsWidget = new Details( m_backend );
        qDebug() << linkNo;
        m_repoLayouts.at( linkNo )->addWidget( detailsWidget );
        m_detailsWidgets.replace( linkNo, detailsWidget );
        m_visible.replace( linkNo, true);
        m_detailsLabels.at( linkNo )->setText( QString( "<a href = %1>Hide Details</a>" ).arg( linkNo ) );
    }
}

void FirstScreen::untrusedRepoDetails( QString link )
{
}
