//      Copyright 2012 Saurabh Sood <saurabh@saurabh.geeko>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//      


#include "fakebackend.h"

FakeBackend::FakeBackend( QWidget *main  )
{
    m_main = main;
}

void FakeBackend::install()
{
    QString packList = "";
    QString repoList = "";

    foreach( QUrl iter, repositories() ) {
        repoList.append( QString( iter.toString() ) );
        repoList.append( "\n" );
    }

    foreach ( QString iter, packages() ) {
        packList.append( iter );
        packList.append( "\n ");
    }

    QMessageBox::about( m_main, "One Click Installer", QString( "The following repositories will be added \n %1" ).arg( repoList ) );
    QMessageBox::about( m_main, "One Click Installer", QString( "The following packages will be installed \n %1" ).arg( packList ) );
    emit installationCompleted();
}

void FakeBackend::callBackendHelper()
{
    install();
}

bool FakeBackend::exists( const QString& url )
{
    return false;
}
