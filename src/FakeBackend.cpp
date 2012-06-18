#include "FakeBackend.h"

FakeBackend::FakeBackend( QWidget *main  )
{
    m_main = main;
}

void
FakeBackend::install()
{
	QString packList = "";
	QString repoList = "";

	foreach( QUrl iter, repositories() ){
		repoList.append( QString( iter.toString() ) );
		repoList.append( "\n" );
	}
	foreach ( QString iter, packages() ){
		packList.append( iter );
		packList.append( "\n ");
	}
	QMessageBox::about( m_main, "One Click Installer", QString( "The following repositories will be added \n %1" ).arg( repoList ) );
	QMessageBox::about( m_main, "One Click Installer", QString( "The following packages will be installed \n %1" ).arg( packList ) );
}

