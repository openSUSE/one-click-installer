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

    //Code to install packages

    zypp::ZYpp::Ptr zypp_pointer = zypp::getZYpp();
    zypp_pointer->initializeTarget( "/" );
    zypp::ResPoolProxy selectablePool( zypp::ResPool::instance().proxy() );

    foreach( QString package, packages() ) {
        zypp::ui::Selectable::Ptr s = zypp::ui::Selectable::get( package.toStdString() );
        zypp::PoolItem p = s->highestAvailableVersionObj();
        s->setCandidate( p );
        p.status().setToBeInstalled( zypp::ResStatus::USER );
    }

    zypp::ResPool pool = zypp::ResPool::instance();
    bool resolved = false;
    resolved = pool.resolver().resolvePool();

    if( !resolved ) {
        std::cout << "Failed to Resolve Pool" << std::endl;
        m_errorCode = 2;
        return;
    } else {
        std::cout << "Resolved Pool" << std::endl;
        //Perform Instalation
        zypp::ZYppCommitPolicy policy;
        policy.restrictToMedia( 0 );
        policy.downloadMode( zypp::DownloadInHeaps );
        policy.syncPoolAfterCommit();
        zypp::ZYppCommitResult result = zypp_pointer->commit( policy );

        if( result.allDone() ) {
            std::cout << "Installation Succeeded" << std::endl;
        } else {
            std::cout << "Installation did not succeed" << std::cout;
            m_errorCode = 3;
        }
    }
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

        if( !exists( url ) ) {
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
        m_manager->buildCache( repoInfo );
        m_manager->loadFromCache( repoInfo );
    }
}

bool Backend::exists( std::string repo )
{
    std::cout << "Parameter is " << repo << std::endl;
    std::list< zypp::RepoInfo > repoList = std::list< zypp::RepoInfo >( m_manager->repoBegin(), m_manager->repoEnd() );

    for( std::list< zypp::RepoInfo >::iterator it = repoList.begin(); it != repoList.end(); it++ ){
        //std::cout <<std::endl<<"Repo URL is " << it->url().asString();
        if( repo.compare( it->url().asString() ) == 0 )
                return true;
    }

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
    emit installationStarted();
}

void Backend::finished( int v )
{
    qDebug() << "helper finished";
    emit installationCompleted();
}
