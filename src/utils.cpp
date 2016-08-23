/***********************************************************************************
 *  One Click Installer makes it easy for users to install software, no matter
 *  where that software is located.
 *
 *  Copyright (C) 2016  Shalom <shalomray7@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************************
 *  This program's developed as part of GSoC - 2016
 *  Project: One Click Installer
 *  Mentors: Antonio Larrosa, and Cornelius Schumacher
 *  Organization: OpenSUSE
 *  Previous Contributor: None
 *  Special Mention: Thanks to Michael Andres for lending a helping hand
 ***********************************************************************************/


#include <zypp/ResKind.h>
#include <zypp/base/Algorithm.h>
#include <zypp/ZYppFactory.h>
#include <zypp/ResPool.h>
#include <zypp/PoolItemBest.h>
#include <zypp/base/Easy.h>
#include "utils.h"

// Define static variables here
scoped_ptr<RepoManager> ZypperUtils::s_repoManager( new RepoManager( Pathname( "/tmp/ociRepos" ) ) );
RepoInfo ZypperUtils::s_repoInfo;
KeyRingReceive ZypperUtils::s_keyReceiveReport;

//Methods
/************************************* HELPER FUNCTIONS **************************************/
void ZypperUtils::initRepository( const string & repoName, const string & repoUrl )
{
    //KeyRing - Handling of gpg keys
    s_keyReceiveReport.connect();
    addRepository( repoUrl, repoName );
    //TRUST_KEY_TEMPORARILY - we need only the key information 
    KeyRing::setDefaultAccept( KeyRing::TRUST_KEY_TEMPORARILY );
    qDebug() << "TRUST_KEY_TEMPORARILY accepted";
    refreshRepoManager();
    //load cache
    try {
	qDebug() << "Loading resolvables from " << s_repoInfo.alias().c_str();
	s_repoManager->loadFromCache( s_repoInfo );      
    }
    catch (const Exception& exp)
    {
	// cachefile has old format or is corrupted: try to rebuild it
	s_repoManager->cleanCache( s_repoInfo );
	s_repoManager->buildCache( s_repoInfo );
	s_repoManager->loadFromCache( s_repoInfo );
    }
    s_keyReceiveReport.disconnect();
    
    qDebug() << "===================================================";
    qDebug() << "Alias: " << s_repoInfo.alias().c_str();
    qDebug() << "Url: " << s_repoInfo.url().asString().c_str();
    qDebug() << "===================================================";
}

void ZypperUtils::initRepoInfo( const string & repoUrl, const string& repoAlias )
{
    s_repoInfo.addBaseUrl( Url( repoUrl ) );
    s_repoInfo.setName( repoAlias );
    s_repoInfo.setAlias( repoAlias );
    s_repoInfo.setGpgCheck( true );
    s_repoInfo.setAutorefresh( true );
}

void ZypperUtils::refreshRepoManager() 
{
    qDebug() << "Refreshing Repo: " << s_repoInfo.alias().c_str();
    s_repoManager->refreshMetadata( s_repoInfo, RepoManager::RefreshForced );
    s_repoManager->buildCache( s_repoInfo );
}

void ZypperUtils::addRepository( const string & repoUrl, const string& repoAlias )
{
    initRepoInfo( repoUrl, repoAlias );   
    // Add it to the repoManager if not already present
    if ( !s_repoManager->hasRepo( repoAlias ) ) {
	s_repoManager->addRepository( s_repoInfo );
	qDebug() << "added repo";
    }
}

bool ZypperUtils::exists(const string & repoUrl)
{
    RepoManager rM;
    for ( RepoInfo& nrepo : rM.knownRepositories() ) {
	if ( !nrepo.enabled() )
	    continue;
	if ( nrepo.url().schemeIsVolatile() )
	    continue;
	if ( repoUrl == nrepo.url().asString() )
	    return true;
    }
    return false;
}

KeyRingReceive ZypperUtils::keyReport()
{
    return s_keyReceiveReport;
}

void ZypperUtils::initSystemRepos()
{
    // Sync the current repo set
    for ( RepoInfo& repo : s_repoManager->knownRepositories() ) {
	if ( !repo.enabled() )
	    continue;
	
	// Often volatile media are sipped in automated environments
	// to avoid media change requests:
	if ( repo.url().schemeIsVolatile() )
	    continue;
	bool refreshNeeded = false;
	if ( repo.autorefresh() )  { // test whether to autorefresh repo metadata
	    for ( const Url& url : repo.baseUrls() ) {
		try {
		    if (s_repoManager->checkIfToRefreshMetadata( repo, url ) == RepoManager::REFRESH_NEEDED ) {
			qDebug() << "Need to autorefresh repository " << repo.alias().c_str();
			refreshNeeded = true;
		    }
		    break;	// exit after first successful checkIfToRefreshMetadata
		}
		catch ( const Exception& exp ) {} // Url failed, try next one
	    }	  
	}
	
	// Initial metadata download or cache refresh 
	if ( !s_repoManager->isCached( repo ) || refreshNeeded ) {
	    qDebug() << "Refreshing repo... " << repo.asUserString().c_str();
	    if ( s_repoManager->isCached( repo ) )
		s_repoManager->cleanCache( repo );
	    s_repoManager->refreshMetadata( repo );
	    s_repoManager->buildCache( repo );
	}
	
	// load cache
	try {
	    qDebug() << "Loading resolvables from " << repo.alias().c_str() << endl;
	    s_repoManager->loadFromCache( repo );	// load available packages to pool
	}
	catch (const Exception& exp ) {
	    // cachefile has old format (or is corrupted): try to rebuild it
	    s_repoManager->cleanCache( repo );
	    s_repoManager->buildCache( repo );
	    s_repoManager->loadFromCache( repo );
	}
    }
}

void ZypperUtils::resetRepoManager( const Pathname & sysRoot )
{
    /* Need to remove temporary PoolItems (loaded into pool when querying package information (metadata))
     * from the ResPool to avoid unnecessary conflicts.
     * 
     * The following code snippet does the work but needs refinement
     * if ( !s_zypp->pool().empty() ) {
     * 	   Repository repo = s_zypp->pool().reposFind( "temp" );
     *     repo.eraseFromPool();
     * }
     * 
     * Reminder To Me (Shalom): Investigate reposEraseAll() [sat/Pool.h]
     */
    
    /* Initialize s_repoManager with sysRoot. This step is necessary to add repo
     * to system repos
     */
    s_repoManager.reset();
    try {
	s_repoManager.reset( new RepoManager( sysRoot ) );
    }
    catch ( const Exception& exp ) {
	qDebug() << "Oops! Failed to init repo manager";
	throw;
    }
}
