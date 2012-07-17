#include "mainheader.h"

MainHeader::MainHeader( QWidget *parent )
{
    setStyleSheet( "background-color : rgb(251,248,241)" );
    m_statusLabel = new QLabel( "This installer will install and download packages" );

    const QPixmap icon( "/usr/share/icons/oneclickinstall.png" );
    m_icon = new QLabel;
    m_icon->setPixmap( icon );

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget( m_icon );
    mainLayout->addSpacing( 100 );
    mainLayout->addWidget( m_statusLabel );

    setLayout( mainLayout );
    show();
}

void MainHeader::changeStatusLabel( int repoCount, int packageCount )
{
    m_statusLabel->setText( QString( "This installer will download and install %1 packages from %2 sources" ).arg( packageCount ).arg( repoCount ) );
}
