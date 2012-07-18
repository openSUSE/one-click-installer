#include "mainwindow.h"
MainWindow::MainWindow( const QString& filename, QString tmpFileName, bool fakeRequested, QObject *parent )
{
    setStyleSheet( "background-color : rgb(251,248,241)" );
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
