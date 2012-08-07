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


#include "repositorymetadata.h"
#include <zypp/Capabilities.h>
#include <zypp/sat/WhatProvides.h>
#include <zypp/sat/Solvable.h>
#include <zypp/RepoManager.h>
#include <zypp/misc/DefaultLoadSystem.h>
#include <zypp/ZYppFactory.h>
#include <zypp/ui/Selectable.h>
#include <zypp/ResObject.h>
#include <zypp/ResKind.h>
#include <zypp/base/LogTools.h>
#include <zypp/base/LogControl.h>
#include <zypp/ByteCount.h>
#include "keyringcallbacks.h"

zypp::KeyRingCallbacks m_keyRing;

RepositoryMetadata::RepositoryMetadata(OCI::Repository *repository )
{
    zypp::RepoManagerOptions options( "/tmp" );
    zypp::RepoInfo repo;
    zypp::RepoManager repoManager( options );
    repo.addBaseUrl( zypp::Url( repository->url().toStdString() ) );
    zypp::KeyRing::setDefaultAccept( zypp::KeyRing::TRUST_KEY_TEMPORARILY );
    repo.setGpgCheck( true );
    repo.setAlias( repository->url().toStdString() );
    repo.setEnabled( true );
    repo.setAutorefresh( true );
    try{
        repoManager.cleanCache( repo );
        repoManager.refreshMetadata( repo );
    } catch( zypp::repo::RepoException s) {
        std::cout << s;
    }
    repoManager.cleanMetadata( repo );
}

QString RepositoryMetadata::created()
{
    return QString::fromStdString( m_keyRing.created() );
}

QString RepositoryMetadata::expires()
{
    return QString::fromStdString( m_keyRing.expires() );
}

QString RepositoryMetadata::fingerprint()
{
    return QString::fromStdString( m_keyRing.fingerprint() );
}

QString RepositoryMetadata::id()
{
    return QString::fromStdString( m_keyRing.id() );
}
