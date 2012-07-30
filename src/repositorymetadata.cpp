#include "repositorymetadata.h"

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
