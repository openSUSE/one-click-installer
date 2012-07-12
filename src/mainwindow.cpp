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

    m_firstScreen = new FirstScreen( m_backend, m_tmpFileName, m_screenStack, filename, this );
    Summary *installSummary = new Summary( m_backend, m_screenStack, m_tmpFileName );
    InstallScreen *installer = new InstallScreen( m_backend, m_screenStack, m_tmpFileName );

    m_screenStack->addWidget( m_firstScreen );
    m_screenStack->addWidget( installSummary );
    m_screenStack->addWidget( installer );

    m_screenStack->setCurrentIndex( 0 );

    //QObject::connect(m_stageWidget, SIGNAL(destroyed()), this, SLOT(close()));

//    mainLayout->addLayout( detailsLayout );
    mainLayout->addWidget( m_header );
    mainLayout->addLayout( m_screenStack );
    setLayout( mainLayout );
    setWindowTitle( "One Click Install" );
    setWindowIcon( QIcon("/usr/share/icons/oneclickinstall.png") );
    show();
}
