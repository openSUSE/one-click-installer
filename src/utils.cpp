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
#include "utils.h"

// Define static variables here
RepoManagerOptions* ZypperUtils::s_repoManagerOpts = new RepoManagerOptions("/tmp");
RepoManager* ZypperUtils::s_repoManager = new RepoManager(*s_repoManagerOpts);
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

void ZypperUtils::initRepoInfo( const string& repoUrl, const string& packageName )
{
    string alias = packageName + "-repo";
    s_repoInfo.addBaseUrl( Url( repoUrl ) );
    s_repoInfo.setName( alias );
    s_repoInfo.setAlias( alias );
    s_repoInfo.setGpgCheck( true );
    s_repoInfo.setAutorefresh( true );
}

void ZypperUtils::refreshRepoManager() 
{
    cout << "Refreshing Repo: " << s_repoInfo.alias() << endl;
    s_repoManager->refreshMetadata( s_repoInfo, RepoManager::RefreshForced );
    s_repoManager->buildCache( s_repoInfo );
}

void ZypperUtils::initRepoManager( const string& repoUrl, const string& packageName )
{
    string alias = packageName + "-repo";
    initRepoInfo( repoUrl, packageName );   
    // Add it to the repoManager if not already present
    if ( !s_repoManager->hasRepo( alias ) ) {
	s_repoManager->addRepository( s_repoInfo );
	cout << "added repo" << endl;
    }
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
    for( PoolQuery::Selectable_iterator it = q.selectableBegin(); it != q.selectableEnd(); ++it ) {
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
void ZypperUtils::markPackagesForInstallation( const string& repoUrl, const string& packageName )
{
    Pathname sysRoot( "/" );
    // acquire initial zypp lock
    s_zypp = getZYpp(); 
    
    makeNecessaryChangesToRunAsRoot( sysRoot );
    initTarget( sysRoot );
    initRepoManager( repoUrl, packageName );
    
    //TRUST_AND_IMPORT_KEY - we are adding this to the system repos
    KeyRing::setDefaultAccept( KeyRing::TRUST_AND_IMPORT_KEY );
    cout << "TRUST_AND_IMPORT_KEY accepted" << endl;
    refreshRepoManager();
    
    //load cache
    try {
	cout << "Loading resolvables from " << s_repoInfo.alias() << endl;
	s_repoManager->loadFromCache( s_repoInfo );      
    }
    catch (const Exception& exp) {
	// cachefile has old format or is corrupted: try to rebuild it
	s_repoManager->cleanCache( s_repoInfo );
	s_repoManager->buildCache( s_repoInfo );
	s_repoManager->loadFromCache( s_repoInfo );
    }
    
    // select/specify package(s) to install
    PoolQuery q;
    q.addKind( ResKind::package );
    q.addAttribute( sat::SolvAttr::name, packageName );
    q.setMatchExact();
    
    PoolItemBest bestMatches(q.begin(), q.end());    
    if ( !bestMatches.empty() ) {
	for ( PoolItemBest::iterator it = bestMatches.begin(); it != bestMatches.end(); ++it ) {
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
  
    unsigned int attempt = 0;
    while ( !s_zypp->resolver()->resolvePool() ) {
	++attempt;
	cout << "Solving dependencies: " << attempt << ". attempt failed" << endl;
	const ResolverProblemList& problems( s_zypp->resolver()->problems() );
	cout << problems.size() << " problems found..." << endl;
	
	ProblemSolutionList toTry;
	unsigned int probNo = 0;
	for (const auto& probPtr : problems) {
	    cout << "Problem " << ++probNo << ": " << probPtr->description() << endl;
	    const ProblemSolutionList & solutions = probPtr->solutions();
	    unsigned int solNo = 0;
	    for ( const auto & solPtr : solutions ) {
		if (solNo == 0) {
		  solNo++;
		  continue;
		}
		cout << "  Solution " << solNo << ": " << solPtr->description() << endl;
		//toTry.push_back( solPtr);
	    }
	}
	
	if ( !toTry.empty() )
	{
	    cout << "Apply selected solutions..." << endl;
	    s_zypp->resolver()->applySolutions( toTry );
	    cout << "Solving dependencies..." << endl;
	    continue;
	}
	// otherwise give up
      throw "Solving dependencies failed: Giving up!";
    }
    cout << "Dependencies solved" << endl; */
    
    return s_zypp->resolver()->resolvePool();
}

bool ZypperUtils::commitChanges()
{
    cout << "Selectable summary (grouped by name):" << endl;
    for ( const ui::Selectable_Ptr & sel : s_zypp->pool().proxy() )
    {
	if ( sel->toModify() )
	cout << "  " << sel << endl;
    }
    
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
    /* Caller: ZypperUtils::install()
     * Can be absolutely certain that s_repoManagerOpts, and s_repoManager
     * are already intialized [to display information]
     */
    try {
	delete s_repoManagerOpts;
	delete s_repoManager;
    }
    catch ( const Exception& exp ) {
	cout << "Oops! You're trying to delete the non-existent. ;)" << endl;
    }

    // Initialize s_repoManager with sysRoot. This step is necessary to add repo
    // to system repos
    try {
	s_repoManager = new RepoManager( sysRoot );
    }
    catch ( const Exception& exp ) {
	cout << "Allocating memory to s_repoManager Failed" << endl;
    }
}

ZYpp::Ptr ZypperUtils::zyppPtr()
{
    return s_zypp;
}
