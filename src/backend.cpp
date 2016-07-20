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
 
#include "backend.h"

void Backend::install()
{
    addRepositories();

    system( "zypper refresh" );

    QString zypper_command = "2>&1 zypper -x -n in";
    foreach( QString package, packages() ) {
        zypper_command.append( QString( " %1" ).arg( package ) );
    }

    m_adaptor = new DBusAdaptor( this );
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService( "org.opensuse.oneclickinstaller" );
    connection.registerObject( "/", this );

    zypper_command.append( " | grep -o --line-buffered \"percent=\\\"[0-9]*\\\"\" | " );
    zypper_command.append( "while read line; do echo \"$line\" > /var/log/oneclick.log; echo \"$line\" > /var/log/oneclick.log; done" );

    QProcess::execute( zypper_command.toStdString().c_str() );
}

Backend::Backend()
{
    m_manager = new zypp::RepoManager;
    m_ptr = new zypp::ZYpp::Ptr;
    m_keyRingManager = new zypp::KeyRingCallbacks;
    m_errorCode = 0;
}

void Backend::addRepositories()
{
    qDebug() << "add repos called";

    foreach( QUrl iter, repositories() ) {
        zypp::RepoInfo repoInfo;
        std::string url = iter.toString().toStdString();
        repoInfo.addBaseUrl( zypp::Url( url ) );
        repoInfo.setAlias( url );
        repoInfo.setGpgCheck( true );

        zypp::KeyRing::setDefaultAccept( zypp::KeyRing::TRUST_AND_IMPORT_KEY );

        if( !exists( QString::fromStdString( url ) ) ) {
            try {
                m_manager->addRepository( repoInfo );
            } catch( zypp::repo::RepoMetadataException e ) {
                m_errorCode = 4;
            }
            catch( zypp::repo::RepoException ) {
                m_errorCode = 5;
            }
        }
        else
            qDebug() << "Repository Exists";
//        m_manager->buildCache( repoInfo );
//        m_manager->loadFromCache( repoInfo );
    }
}

bool Backend::exists(const QString& repo )
{
    qDebug() << "Parameter is " << repo;
    std::list< zypp::RepoInfo > repoList = std::list< zypp::RepoInfo >( m_manager->repoBegin(), m_manager->repoEnd() );

    for( std::list< zypp::RepoInfo >::iterator it = repoList.begin(); it != repoList.end(); it++ ){
        //std::cout <<std::endl<<"Repo URL is " << it->url().asString();
        if( repo.toStdString().compare( it->url().asString() ) == 0 ) {
            qDebug() << "returning true";
            return true;
        }
    }

    qDebug() << "returning false";
    return false;
}

void Backend::callBackendHelper()
{
    QString command( "xdg-su -u root -c \"/usr/sbin/oneclickhelper " );
    command.append( getFileName() );
    command.append( "\"" );
//    qDebug() << system( command.toLocal8Bit().data() );
    m_process = new QProcess;

    QObject::connect( m_process, SIGNAL( finished( int) ), this, SLOT( finished( int ) ) );
    QObject::connect( m_process, SIGNAL( started() ), this, SLOT(started()) );

    m_process->start( command );
}

int Backend::errorCode()
{
    return m_errorCode;
}

void Backend::started()
{
    qDebug() << "helper started";
    emit checkForConflicts();
}

void Backend::finished( int v )
{
    if( v != 0 ) {
        qDebug() << "helper finished but failed";
        emit installationFailed();
    } else {
        qDebug() << "helper finished successfully";
        emit installationCompleted();
    }
}

void Backend::KillBackend()
{
    system( "kill $(pgrep zypper)" );
}
