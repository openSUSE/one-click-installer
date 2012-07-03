#include "backend.h"

void Backend::install()
{
    addRepositories();
}

Backend::Backend()
{
    m_manager = new zypp::RepoManager;
    m_ptr = new zypp::ZYpp::Ptr;
    m_keyRingManager = new zypp::KeyRingCallbacks;
}

void Backend::addRepositories()
{
    qDebug() << "add repos called";
    foreach( QUrl iter, repositories() ){
        zypp::RepoInfo repoInfo;
        std::string url = iter.toString().toStdString();
        repoInfo.addBaseUrl( zypp::Url( url ) );
        repoInfo.setAlias( url );
        repoInfo.setGpgCheck( true );

        if( !exists( url ) )
            m_manager->addRepository( repoInfo );
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
