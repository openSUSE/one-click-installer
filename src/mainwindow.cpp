#include "mainwindow.h"
MainWindow::MainWindow( const QString& filename, QString tmpFileName, bool fakeRequested, QObject *parent )
{
    setStyleSheet( "background-color : rgb(251,248,241)" );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    m_showSettings = new QPushButton( "Settings" );
    m_cancel = new QPushButton( "Cancel" );
    m_install = new QPushButton( "Install" );

    buttonLayout->addWidget( m_showSettings );
    buttonLayout->addSpacing( 100 );
    buttonLayout->addWidget( m_cancel );
    buttonLayout->addSpacing( 10 );
    buttonLayout->addWidget( m_install );

    QObject::connect( m_showSettings, SIGNAL( clicked() ), this, SLOT( showSettings() ) );
    QObject::connect( m_install, SIGNAL( clicked() ), this, SLOT( performInstallation() ) );
    QObject::connect( m_cancel, SIGNAL( clicked()), this, SLOT( close() ) );

    m_tmpFileName = new QString( tmpFileName );
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_screenStack = new QStackedLayout;

    //Set up the backend
    if( fakeRequested ) {
        m_backend = new FakeBackend( this );
        m_fakeRequested = true;
    } else {
        m_backend = new Backend;
        m_fakeRequested = false;
    }

    m_header = new MainHeader;

    m_firstScreen = new FirstScreen( m_backend, m_tmpFileName, filename, this );
    Summary *installSummary = new Summary( m_backend, m_tmpFileName );
    InstallScreen *installer = new InstallScreen( m_backend, m_tmpFileName );

    m_warning = new	QLabel( "<b>Be careful!</b> Some Sources are not currently known. Installing<br />software requires trusting these sources" );
    m_warning->setStyleSheet( "border : 1px solid rgb(196,181,147); background-color: rgb(253, 227, 187); border-radius : 10px" );
    m_warning->setContentsMargins( 10,10,10,10 );

    QScrollArea *scroll = new QScrollArea;
    scroll->setFrameShape( QFrame::NoFrame );
    scroll->setWidget( m_firstScreen );
    scroll->setWidgetResizable( true );

    m_screenStack->addWidget( scroll );
    m_screenStack->addWidget( installSummary );
    m_screenStack->addWidget( installer );

    m_screenStack->setCurrentIndex( 0 );

    mainLayout->addWidget( m_header );
    mainLayout->addLayout( m_screenStack );
    mainLayout->addSpacing( 5 );
    mainLayout->addWidget( m_warning );
    mainLayout->addSpacing( 20 );
    mainLayout->addLayout( buttonLayout );

    setLayout( mainLayout );
    setWindowTitle( "One Click Install" );
    setWindowIcon( QIcon("/usr/share/icons/oneclickinstall.png") );

    QObject::connect( m_firstScreen, SIGNAL( showNextScreen( int ) ), this, SLOT( showNextScreen( int ) ) );
    QObject::connect( installSummary, SIGNAL( showNextScreen( int ) ), this, SLOT( showNextScreen( int ) ) );
    QObject::connect( m_firstScreen, SIGNAL( countChanged( int, int )), this, SLOT( updateCount( int, int ) ) );
    QObject::connect( this, SIGNAL( countChanged( int, int ) ), m_header, SLOT( changeStatusLabel( int, int) ) );

    show();
}

void MainWindow::showNextScreen( int index )
{
    m_screenStack->setCurrentIndex( index );
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
    if( m_settings.value( "proposal", 1 ).toInt() == 1 ) {
        m_screenStack->setCurrentIndex( 1 );
    } else {
        m_screenStack->setCurrentIndex( 2 );
    }
    m_install->hide();
    m_showSettings->hide();
    m_cancel->hide();
}
