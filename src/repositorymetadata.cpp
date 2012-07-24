#include "repositorymetadata.h"

RepositoryMetadata::RepositoryMetadata( std::string url )
{
    zypp::RepoManagerOptions options( "/tmp" );
    zypp::RepoInfo repo;
    zypp::RepoManager repoManager( options );
    repo.addBaseUrl( zypp::Url( url ) );
    zypp::KeyRing::setDefaultAccept( zypp::KeyRing::TRUST_KEY_TEMPORARILY );
    repo.setGpgCheck( true );
    repo.setAlias( url );
    repo.setEnabled( true );
    repo.setAutorefresh( true );
    try{
    repoManager.refreshMetadata( repo );
    } catch( zypp::repo::RepoException s) {
        std::cout << s;
    }
    repoManager.cleanMetadata( repo );
}

std::string RepositoryMetadata::created()
{
    return m_keyRing.created();
}

std::string RepositoryMetadata::expires()
{
    return m_keyRing.expires();
}

std::string RepositoryMetadata::fingerprint()
{
    return m_keyRing.fingerprint();
}

std::string RepositoryMetadata::id()
{
    return m_keyRing.id();
}
