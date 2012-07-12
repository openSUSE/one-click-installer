#include "mainwindow.h"
MainWindow::MainWindow( const QString& filename, QString tmpFileName, bool fakeRequested, QObject *parent)
{
    setStyleSheet( "background-color : rgb(251,248,241)" );
    m_info = new QLabel( "This installer will install and download packages" );
    m_tmpFileName = new QString( tmpFileName );
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *detailsLayout = new QHBoxLayout;
    const QPixmap icon( "/usr/share/icons/oneclickinstall.png" );
    QLabel *imgView = new QLabel;
    imgView->setPixmap( icon );
    detailsLayout->addWidget( imgView );
    detailsLayout->addWidget( m_info );

    //Set up the backend
    if( fakeRequested ) {
        m_backend = new FakeBackend( this );
        m_fakeRequested = true;
    } else {
        m_backend = new Backend;
        m_fakeRequested = false;
    }

    m_firstScreen = new FirstScreen( m_backend, m_tmpFileName, m_fakeRequested, m_stageWidget, filename, this );
    m_stageWidget = m_firstScreen;
    QObject::connect(m_stageWidget, SIGNAL(destroyed()), this, SLOT(close()));

    mainLayout->addLayout( detailsLayout );
    mainLayout->addWidget( m_stageWidget );
    setLayout( mainLayout );
    setWindowTitle( "One Click Install" );
    setWindowIcon( QIcon("/usr/share/icons/oneclickinstall.png") );
    show();
}
