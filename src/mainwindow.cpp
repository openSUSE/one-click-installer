#include "mainwindow.h"
MainWindow::MainWindow( const QString& filename, QObject *parent)
{
    m_info = new QLabel( "This installer will install and download packages" );
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *detailsLayout = new QHBoxLayout;
    const QPixmap icon( "res/oneclickinstall.png" );
    QLabel *imgView = new QLabel;
    imgView->setPixmap( icon );
    detailsLayout->addWidget( imgView );
    detailsLayout->addWidget( m_info );
    //Set up the backend
    m_backend = new FakeBackend( this );
    m_firstScreen = new FirstScreen( m_backend, m_stageWidget, filename, this );
    m_stageWidget = m_firstScreen;
    QObject::connect(m_stageWidget, SIGNAL(destroyed()), this, SLOT(close()));
    mainLayout->addLayout( detailsLayout );
    mainLayout->addWidget( m_stageWidget );
    setLayout( mainLayout );
    setWindowTitle( "One Click Install" );
    show();
}
