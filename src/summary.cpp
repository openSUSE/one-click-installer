#include "summary.h"

Summary::Summary( PackageBackend *backend, QObject *parent )
{
    m_backend = backend;
    m_installationSummary = new QTextBrowser;
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
    show();
}
