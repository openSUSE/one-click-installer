#include "summary.h"
#include <unistd.h>

Summary::Summary(PackageBackend *backend, QString *tmpFileName, QObject *parent )
{
    m_backend = backend;
    m_tmpFileName = tmpFileName;
    m_installationSummary = new QTextBrowser;
    m_installationSummary->addScrollBarWidget( new QScrollBar, Qt::AlignRight );
    m_continue = new QPushButton( "Continue Installation ");
    m_cancel = new QPushButton( "Cancel ");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    mainLayout->addWidget( m_installationSummary );
    buttonsLayout->addSpacing( 100 );
    buttonsLayout->addWidget( m_cancel );
    buttonsLayout->addSpacing( 20 );
    buttonsLayout->addWidget( m_continue );
    mainLayout->addLayout( buttonsLayout );
    setLayout( mainLayout );

    foreach( QUrl repos, m_backend->repositories() ) {
        m_installationSummary->append( QString( "Add Source <b>%1</b>" ).arg( repos.toString() ) );
    }

    foreach( const QString package, m_backend->packages() ) {
        m_installationSummary->append( QString( "Install <b>%1</b>" ).arg( package ) );
    }

    QObject::connect( m_continue, SIGNAL( clicked() ), this, SLOT( continueInstallation() ) );
    QObject::connect( m_cancel, SIGNAL( clicked() ), this, SLOT( cancel() ) );
}

void Summary::continueInstallation()
{
    emit showNextScreen( 2 );
}

void Summary::cancel()
{
    exit( 0 );
}
