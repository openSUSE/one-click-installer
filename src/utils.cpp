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
scoped_ptr<RepoManager> ZypperUtils::s_repoManager( new RepoManager( Pathname( "/tmp" ) ) );
RepoInfo ZypperUtils::s_repoInfo;
KeyRingReceive ZypperUtils::s_keyReceiveReport;
ZYpp::Ptr ZypperUtils::s_zypp;

//Methods
/************************************* HELPER FUNCTIONS **************************************/
void ZypperUtils::initRepository( const string& repoUrl )
{
    //KeyRing - Handling of gpg keys
    s_keyReceiveReport.connect();
    initRepoManager( repoUrl );
    //TRUST_KEY_TEMPORARILY - we need only the key information 
    KeyRing::setDefaultAccept( KeyRing::TRUST_KEY_TEMPORARILY );
    cout << "TRUST_KEY_TEMPORARILY accepted" << endl;
    refreshRepoManager();
    //load cache
    try {
	cout << "Loading resolvables from " << s_repoInfo.alias() << endl;
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
    
    cout << "===================================================" << endl;
    cout << "Alias: " << s_repoInfo.alias() << endl;
    cout << "Url: " << s_repoInfo.url() << endl;
    cout << "===================================================" << endl;
}

void ZypperUtils::initRepoInfo( const string& repoUrl, const string& repoAlias )
{
    s_repoInfo.addBaseUrl( Url( repoUrl ) );
    s_repoInfo.setName( repoAlias );
    s_repoInfo.setAlias( repoAlias );
    s_repoInfo.setGpgCheck( true );
    s_repoInfo.setAutorefresh( true );
}

void ZypperUtils::refreshRepoManager() 
{
    cout << "Refreshing Repo: " << s_repoInfo.alias() << endl;
    s_repoManager->refreshMetadata( s_repoInfo, RepoManager::RefreshForced );
    s_repoManager->buildCache( s_repoInfo );
}

void ZypperUtils::initRepoManager( const string& repoUrl, const string& repoAlias )
{
    initRepoInfo( repoUrl, repoAlias );   
    // Add it to the repoManager if not already present
    if ( !s_repoManager->hasRepo( repoAlias ) ) {
	s_repoManager->addRepository( s_repoInfo );
	cout << "added repo" << endl;
    }
}

bool ZypperUtils::exists(const string& repoUrl)
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

/************************************* ZYPPER INFO **************************************/
PoolItem ZypperUtils::queryMetadataForPackage( const string& packageName )
{
    PoolQuery q;
    q.addKind( ResKind::package );
    q.addAttribute( sat::SolvAttr::name, packageName );
    q.setMatchExact();

    return packageObject( q );
}
  
PoolItem ZypperUtils::packageObject( const PoolQuery& q )
{
    PoolItem item;
    for_( it, q.selectableBegin(), q.selectableEnd() ) {
	const ui::Selectable& s =  *(*it);   
	// An update candidate object is better than any installed object
	PoolItem updateObject( s.updateCandidateObj() );
	if ( updateObject )
	    item = updateObject;
	else
	    item = s.installedObj();
    }
    return item;
}

KeyRingReceive ZypperUtils::keyReport()
{
    return s_keyReceiveReport;
}

/************************************* ZYPPER INSTALL ************************************/

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
	    for (const Url& url : repo.baseUrls() ) {
		try {
		    if (s_repoManager->checkIfToRefreshMetadata( repo, url ) == RepoManager::REFRESH_NEEDED ) {
			cout << "Need to autorefresh repository " << repo.alias() << endl;
			refreshNeeded = true;
		    }
		    break;	// exit after first successful checkIfToRefreshMetadata
		}
		catch (const Exception& exp) {} // Url failed, try next one
	    }	  
	}
	
	// Initial metadata download or cache refresh 
	if ( !s_repoManager->isCached( repo ) || refreshNeeded ) {
	    cout << "Refreshing repo... " << repo << endl;
	    if ( s_repoManager->isCached( repo ) )
		s_repoManager->cleanCache( repo );
	    s_repoManager->refreshMetadata( repo );
	    s_repoManager->buildCache( repo );
	}
	
	// load cache
	try {
	    cout << "Loading resolvables from " << repo.alias() << endl;
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

void ZypperUtils::markPackagesForInstallation( const string& repoUrl, const string& packageName )
{
    Pathname sysRoot( "/" );
    string repoAlias = packageName + "-repo";
    // acquire initial zypp lock
    s_zypp = getZYpp(); 
    
    makeNecessaryChangesToRunAsRoot( sysRoot );
    initTarget( sysRoot );
    initRepoManager( repoUrl, repoAlias ); // Add repo listed in .ymp file to the system repos
    initSystemRepos(); // Load system repos into ResPool to resolve missing dependencies
    
    // select/specify package(s) to install
    PoolQuery q;
    q.addKind( ResKind::package );
    q.addAttribute( sat::SolvAttr::name, packageName );
    q.setMatchExact();
    
    PoolItemBest bestMatches(q.begin(), q.end());    
    if ( !bestMatches.empty() ) {
	for_( it, bestMatches.begin(), bestMatches.end() ) {
	    ui::asSelectable()( *it )->setOnSystem( *it, ResStatus::USER );
	}
    }
    cout << s_zypp->pool() << endl;   
    cout << "=====================[ pool ready ]=============================" << endl;
}

bool ZypperUtils::resolveConflictsAndDependencies()
{
    // Solve Selection
    cout << "Solving dependencies..." << endl;
    
    /* 
     * The following code commented out is needed for GUI elements.
     * For instance, each solution (solPtr->description()) is displayed as a radio button
     * on the ConflictScreen 

    cout << "Solving dependencies... " << endl;
    unsigned int attempt = 0;
    while ( !s_zypp->resolver()->resolvePool() ) {
      cout << "Solving dependencies: " << ++attempt << ". attemp failed" << endl;
      const ResolverProblemList& problems( s_zypp->resolver()->problems() );
      cout << problems.size() << " problems found..." << endl;
      
      ProblemSolutionList toTry;
      unsigned int probNo = 0;
      for (const auto& probPtr : problems) {
	  cout << "Problem " << ++probNo << ": " << probPtr->description() << endl;
	  const ProblemSolutionList & solutions = probPtr->solutions();
	  unsigned int solNo = 0;
	  for ( const auto & solPtr : solutions ) {
	      char choice;
	      cout << "  Solution " << ++solNo << ": " << solPtr->description() << endl;
	      cout << "Enter your choice: y/n " << endl;
	      cin >> choice;
	      if (choice == 'y' || choice == 'Y') {
		  toTry.push_back( solPtr );	
		  break;
	      }
	      else continue;
	    } 
	}
	
	if ( !toTry.empty() )
	{
	    cout << "Apply selected solutions..." << endl;
	    s_zypp->resolver()->applySolutions( toTry );
	    cout << "Solving dependencies..." << endl;
	    continue;
	}
	throw "Solving dependencies failed: Giving up!";
    }
    cout << "Dependecies solved" << endl;
    
    cout << "Selectable summary (grouped by name):" << endl;
    for ( const ui::Selectable_Ptr & sel : s_zypp->pool().proxy() )
    {
	if ( sel->toModify() )
	cout << "  " << sel << endl;
    }
    */
    
    return s_zypp->resolver()->resolvePool();
}

bool ZypperUtils::commitChanges()
{
    // Commit the changes 
    // Please note that dryRunFlag and zypp::DownloadOnly are for now
    cout << "Committing the changes" << endl;
    bool dryRunFlag = false;
    ZYppCommitPolicy policy;
    if ( !dryRunFlag ) {
	policy.dryRun( true );
	dryRunFlag = true;
    }
    policy.downloadMode( zypp::DownloadOnly );
    
    try {
	ZYppCommitResult result = s_zypp->commit( policy );
	if ( !( result.allDone() || (dryRunFlag && result.noError() ) ) )
	    throw "Incomplete Commit";
	cout << "Commit Succeeded" << endl;
    }
    catch ( const Exception& exp ) {
	cout << "Commit aborted with exception" << endl;
	throw;	
    }
}

void ZypperUtils::initTarget( const Pathname& sysRoot )
{
    cout << "Initializing target at " << sysRoot << endl;
    s_zypp->initializeTarget( sysRoot ); //initialize target
    cout << "Loading target resolvables" << endl;
    s_zypp->getTarget()->load(); // load installed packages to pool
}

void ZypperUtils::makeNecessaryChangesToRunAsRoot( const Pathname& sysRoot )
{
    /* Need to remove temporary PoolItems (loaded into pool when querying package information (metadata))
     * from the ResPool to avoid unnecessary conflicts.
     * 
     * The following code snippet does the work but needs refinement
     * if ( s_zypp->pool().empty() ) {
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
	cout << "Oops! Failed to init repo manager" << endl;
	throw;
    }
}

ZYpp::Ptr ZypperUtils::zyppPtr()
{
    return s_zypp;
}
