#include "summary.h"

Summary::Summary( PackageBackend *backend, QWidget *stageWidget, QObject *parent )
{
    m_backend = backend;
    m_stageWidget = stageWidget;
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

    QObject::connect( m_continue, SIGNAL( clicked() ), this, SLOT( continueInstallation() ) );
    QObject::connect( m_cancel, SIGNAL( clicked() ), this, SLOT( cancel() ) );
    show();
}

void Summary::continueInstallation()
{
    InstallScreen *installer = new InstallScreen( m_backend );
    m_stageWidget->parentWidget()->layout()->addWidget( installer );
    m_stageWidget = installer;
    delete this;
}

void Summary::cancel()
{
    exit( 0 );
}
