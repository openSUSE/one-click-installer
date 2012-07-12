#include "installscreen.h"

InstallScreen::InstallScreen(PackageBackend *backend, QStackedLayout *screenStack, QString *tmpFileName, QObject *parent )
{
    m_backend = backend;
    m_tmpFileName = tmpFileName;
    m_screenStack = screenStack;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVBoxLayout * installLayout = new QVBoxLayout;

    m_installStatus = new QLabel( "Downloading and Installing Packages" );
    m_cancel = new QPushButton( "Cancel Installation" );

    foreach( QString iter, m_backend->packages() ) {
        QHBoxLayout *packageLayout = new QHBoxLayout;
        QLabel *package = new QLabel( iter );
        QProgressBar * progressBar = new QProgressBar;
        progressBar->setMinimum( 0 );
        progressBar->setMaximum( 0 );
        progressBar->setRange( 0, 0 );
        packageLayout->addWidget( package );
        packageLayout->addWidget( progressBar );
        installLayout->addLayout( packageLayout );
    }

    mainLayout->addLayout( installLayout );
    mainLayout->addWidget( m_cancel );
    setLayout( mainLayout );
}

void InstallScreen::showEvent( QShowEvent *s )
{
    m_backend->setFileName( *m_tmpFileName );
    m_backend->callBackendHelper();
}
