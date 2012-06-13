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

bool FakeBackend::performInstallation( const QList< QString >& pack )
{
	QString packList = "";
	foreach ( QString iter, pack ){
		packList.append( iter );
		packList.append( "\n ");
	}
	QMessageBox::about( m_main, "One Click Installer", QString( "The following packages will be installed \n %1" ).arg( packList ) );
	return true;
}

bool FakeBackend::resolveConflicts()
{

}

