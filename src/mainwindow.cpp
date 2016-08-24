#include <QDBusConnection>
#include <klocalizedstring.h>
#include <sys/types.h>
#include <unistd.h>
#include "mainwindow.h"
#include "checkconflictscreen.h"
#include "conflictresolutionscreen.h"
#include "finallogscreen.h"

MainWindow::MainWindow( const QString & filename, const QString & tmpFileName, bool fakeRequested, QObject *parent )
{
    QDBusConnection sysBus = QDBusConnection::systemBus();
  
    setStyleSheet(QString::fromUtf8("background-color : rgb(251,248,241);"));
    
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setMinimumSize( 600, 400 );

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QHBoxLayout *m_warningLayout = new QHBoxLayout;

    m_showSettings = new QPushButton( i18n("Settings") );
    m_cancel = new QPushButton( i18n("Cancel") );
    m_install = new QPushButton( i18n("Install") );
    
    m_conflictCancel = new QPushButton( i18n( "Cancel" ) );
    m_conflictContinueInstallation = new QPushButton( i18n( "Continue Installation" ) );
    m_conflictCancel->hide();
    m_conflictContinueInstallation->hide();
    
    m_finalLogExit = new QPushButton( i18n ( "Exit" ) );
    m_finalLogExit->hide();
    
    buttonLayout->addWidget( m_showSettings );
    buttonLayout->addSpacing( 350 );
    buttonLayout->addWidget( m_conflictCancel );
    buttonLayout->addWidget( m_finalLogExit );
    buttonLayout->addWidget( m_cancel );
    buttonLayout->addSpacing( 10 );
    buttonLayout->addWidget( m_conflictContinueInstallation );
    buttonLayout->addWidget( m_install );

    m_tmpFileName = tmpFileName;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_screenStack = new QStackedLayout;

    //Set up the backend
    if( fakeRequested ) {
        m_backend = new FakeBackend( this );
        m_fakeRequested = true;
    } else {
        m_backend = new BackendOCI( m_tmpFileName );
        m_fakeRequested = false;
    }

    m_header = new MainHeader;

    m_firstScreen = new FirstScreen( m_backend, m_tmpFileName, filename, this );
    qDebug() << "after m_firstScreen";
    Summary *installSummary = new Summary( m_backend, m_tmpFileName );
    qDebug() << "after installSummary";
    InstallScreen *installer = new InstallScreen();
    qDebug() << "after installer";
    CheckConflictScreen *checkForConflictsScreen = new CheckConflictScreen();
    qDebug() << "after checkForConflictsScreen";
    ConflictResolutionScreen *conflictResolutionScreen = new ConflictResolutionScreen();
    qDebug() << "after conflictresolutionscreen";
    FinalLogScreen *finalLogScreen = new FinalLogScreen();
    
    QScrollArea *scroll = new QScrollArea;
    scroll->setFrameShape( QFrame::NoFrame );
    scroll->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scroll->setWidget( m_firstScreen );
    scroll->setWidgetResizable( true );
    
    QScrollArea *scrollConflictScreen = new QScrollArea;
    scrollConflictScreen->setFrameShape( QFrame::NoFrame );
    scrollConflictScreen->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollConflictScreen->setWidget( conflictResolutionScreen );
    scrollConflictScreen->setWidgetResizable( true );
    
    QScrollArea *scrollFinalLogScreen = new QScrollArea;
    scrollFinalLogScreen->setFrameShape( QFrame::NoFrame );
    scrollFinalLogScreen->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollFinalLogScreen->setWidget( finalLogScreen );
    scrollFinalLogScreen->setWidgetResizable( true );
    
    m_screenStack->addWidget( scroll );
    m_screenStack->addWidget( installSummary );
    m_screenStack->addWidget( installer );
    m_screenStack->addWidget( checkForConflictsScreen );
    m_screenStack->addWidget( scrollConflictScreen );
    m_screenStack->addWidget( scrollFinalLogScreen );

    m_screenStack->setCurrentIndex( 0 );

    mainLayout->addWidget( m_header );
    mainLayout->addLayout( m_screenStack );
    mainLayout->addSpacing( 5 );
    mainLayout->addLayout( m_warningLayout );
    mainLayout->addSpacing( 20 );
    mainLayout->addLayout( buttonLayout );

    setLayout( mainLayout );
    setWindowTitle("One Click Installer");
    setWindowIcon( QIcon("/usr/share/icons/hicolor/32x32/apps/oneclickinstall.png") );

    QObject::connect( m_showSettings, SIGNAL( clicked() ), this, SLOT( showSettings() ) );
    QObject::connect( m_install, SIGNAL( clicked() ), this, SLOT( performInstallation() ) );
    QObject::connect( m_cancel, SIGNAL( clicked()), this, SLOT( close() ) );
    QObject::connect( m_firstScreen, SIGNAL( showNextScreen( int ) ), this, SLOT( showNextScreen( int ) ) );
    QObject::connect( installSummary, SIGNAL( showNextScreen( int ) ), this, SLOT( showNextScreen( int ) ) );
    QObject::connect( m_firstScreen, SIGNAL( countChanged( int, int )), this, SLOT( updateCount( int, int ) ) );
    QObject::connect( this, SIGNAL( countChanged( int, int ) ), m_header, SLOT( changeStatusLabel( int, int) ) );
    QObject::connect( m_firstScreen, SIGNAL( sizeUpdated( QString ) ), this, SLOT( updateSize( QString ) ) );
    QObject::connect( m_firstScreen, SIGNAL( packageListInstallableStateToggled( bool ) ), m_install, SLOT( setEnabled( bool ) ) );

    // For Conflict Resolution
    QObject::connect( m_backend, SIGNAL( checkForConflicts() ), this, SLOT( showCheckForConflictsProgress() ) );
    QObject::connect( m_backend, SIGNAL( checkForConflicts() ), m_header, SLOT( showCheckForConflictsHeader() ) );
    
    QObject::connect( m_conflictContinueInstallation, SIGNAL( clicked() ), conflictResolutionScreen, SLOT( setSolutionID() ) );
    QObject::connect( m_conflictCancel, SIGNAL( clicked() ), conflictResolutionScreen, SLOT( cancelInstallation() ) );
    sysBus.connect( QString(), QString(), "org.opensuse.OCIHelper", "hasConflicts", m_header, SLOT( showConflictResolutionHeader() ) );
    sysBus.connect( QString(), QString(), "org.opensuse.OCIHelper", "hasConflicts", this, SLOT( showConflictResolutionScreen() ) );
    // create and instantiate the MediaCallbacks DBus proxy
    sysBus.connect( QString(), QString(), "org.opensuse.OCIHelper", "noConflicts", installer, SLOT( initDBusServices() ) );
    sysBus.connect( QString(), QString(), "org.opensuse.OCIHelper", "noConflicts", m_header, SLOT( installationStarted() ) );
    sysBus.connect( QString(), QString(), "org.opensuse.OCIHelper", "noConflicts", this, SLOT( showInstallationScreen() ) );
    
    // final installation log screen
    sysBus.connect( QString(), QString(), "org.opensuse.OCIHelper", "installationFinished", installer, SLOT( closeLogFile() ) );
    sysBus.connect( QString(), QString(), "org.opensuse.OCIHelper", "installationFinished", finalLogScreen, SLOT( processInstalledPackages( QStringList, bool ) ) );
    sysBus.connect( QString(), QString(), "org.opensuse.OCIHelper", "installationFinished", this, SLOT( showFinalInstallationLogScreen() ) );
    
    QObject::connect( finalLogScreen, SIGNAL( finalLogHeader( int, bool, bool ) ), m_header, SLOT( showFinalInstallationLogHeader( int, bool, bool ) ) );
    QObject::connect( m_finalLogExit, SIGNAL( clicked() ), finalLogScreen, SLOT( close() ) );
 
    show();
}

void MainWindow::showFinalInstallationLogScreen()
{
    // Hide all button except m_finalLogExit
    m_conflictCancel->hide();
    m_conflictContinueInstallation->hide();
    m_finalLogExit->show();
    
    m_screenStack->setCurrentIndex( 5 );
}

void MainWindow::showInstallationScreen()
{
    m_screenStack->setCurrentIndex( 2 );
    m_conflictCancel->hide();
    m_conflictContinueInstallation->hide();
    
}
void MainWindow::showConflictResolutionScreen()
{
    m_screenStack->setCurrentIndex( 4 );
    
    m_install->hide();
    m_showSettings->hide();
    m_cancel->hide();
    
    m_conflictCancel->show();
    m_conflictContinueInstallation->show();
}
void MainWindow::showNextScreen( int index )
{
    m_screenStack->setCurrentIndex( index );
}

void MainWindow::showCheckForConflictsProgress()
{
    m_screenStack->setCurrentIndex( 3 );
    
    m_install->hide();
    m_showSettings->hide();
    m_cancel->hide();
}

void MainWindow::updateCount( int repoCount, int packageCount )
{
    emit countChanged( repoCount, packageCount );
}


void MainWindow::showSettings()
{
    new Settings( &m_settings );
}

void MainWindow::performInstallation()
{
    if( m_settings.value( "proposal" ).toInt() == 1 ) {
        m_screenStack->setCurrentIndex( 1 );
    } else {
        //m_screenStack->setCurrentIndex( 2 );
        m_backend->callBackendHelper();
    }
    m_install->hide();
    m_showSettings->hide();
    m_cancel->hide();
}

void MainWindow::updateSize( const QString & size )
{
    if( m_screenStack->currentIndex() == 0 )
        m_header->updateDetails( size );
}
