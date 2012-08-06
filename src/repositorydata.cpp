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

#include "repositorydata.h"

RepositoryData::RepositoryData()
{
}

void RepositoryData::setAlias( std::string alias )
{
    m_alias = alias;
}

void RepositoryData::setBaseUrl( std::string url )
{
    m_baseUrl = url;
}

void RepositoryData::refreshMetadata()
{
    m_options = new zypp::RepoManagerOptions( "/tmp" );
    m_repoManager = new zypp::RepoManager( *m_options );
    m_repo.addBaseUrl( zypp::Url( m_baseUrl ) );
    m_repo.setGpgCheck( true );
    m_repo.setAlias( m_alias );
    m_repo.setEnabled( true );
    m_repo.setAutorefresh( true );
    try{
        m_repoManager->cleanCache( m_repo );
        m_repoManager->refreshMetadata( m_repo );
    } catch( zypp::repo::RepoException s) {
        std::cout << s;
    }
    m_repoManager->cleanMetadata( m_repo );
    delete m_options;
    delete m_repoManager;
}

zypp::KeyRingCallbacks RepositoryData::keyring()
{
    return m_keyring;
}
