#include "FakeBackend.h"

FakeBackend::FakeBackend( QWidget *main  )
{
    m_allTrusted = false;
    m_main = main;
}

void FakeBackend::addRepository( const QString& url)
{
    QMessageBox::about( m_main, "One Click Installer", QString( "Repository with URL %1 will be added"  ).arg( url  ) );
}

bool FakeBackend::performInstallation()
{

}

bool FakeBackend::resolveConflicts()
{

}

