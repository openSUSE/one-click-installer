#include "FakeBackend.h"

FakeBackend::FakeBackend( QWidget *main  )
{
    m_allTrusted = false;
    m_main = main;
}

void
FakeBackend::setToAddRepository( const QUrl& repo)
{
	m_repoList << repo.toString();
}

void
FakeBackend::addRepositories()
{
	QString repos;
	foreach( QString iter, m_repoList ){
		repos.append( iter );
		repos.append( "\n" );
	}
	QMessageBox::about( m_main, "One Click Installer", QString( "The following repositories will be added\n %1" ).arg( repos ) );
}

void
FakeBackend::performInstallation( const QString& pack )
{
	m_packageList << pack;
}

void
FakeBackend::installPackages()
{
	QString packList = "";
	foreach ( QString iter, m_packageList ){
		packList.append( iter );
		packList.append( "\n ");
	}
	QMessageBox::about( m_main, "One Click Installer", QString( "The following packages will be installed \n %1" ).arg( packList ) );
}

bool
FakeBackend::resolveConflicts()
{

}

