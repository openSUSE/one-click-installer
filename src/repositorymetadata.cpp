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

RepositoryMetadata::RepositoryMetadata(OCI::Repository *repository )
{
    m_repoData = new RepositoryData;
    m_repoData->setAlias( repository->name().toStdString() );
    m_repoData->setBaseUrl( repository->url().toStdString() );
}

QString RepositoryMetadata::created()
{
    return QString::fromStdString( m_repoData->keyring().created() );
}

QString RepositoryMetadata::expires()
{
    return QString::fromStdString( m_repoData->keyring().expires() );
}

QString RepositoryMetadata::fingerprint()
{
    return QString::fromStdString( m_repoData->keyring().fingerprint() );
}

QString RepositoryMetadata::id()
{
    return QString::fromStdString( m_repoData->keyring().id() );
}
