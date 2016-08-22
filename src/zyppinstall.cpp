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
#include <zypp/RepoManager.h>
#include <zypp/Pathname.h>
#include <zypp/FileChecker.h>
#include <zypp/media/MediaException.h>

#include "zyppinstall.h"
#include "utils.h"
#include "runtimedata.h"

ZYpp::Ptr ZyppInstall::s_zypp = getZYpp(); // acquire initial zypp lock

void ZyppInstall::markPackagesForInstallation(const string& packageName)
{
    Pathname sysRoot( "/" );
    initTarget( sysRoot );
    ZypperUtils::initSystemRepos(); // Load system repos into ResPool to resolve missing dependencies
    
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

bool ZyppInstall::resolveConflictsAndDependencies()
{
    // Solve Selection
    cout << "Solving dependencies..." << endl;
    return s_zypp->resolver()->resolvePool();
}

bool ZyppInstall::commitChanges()
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
	// set runtime data
	// RuntimeData::instance()->setCommitPackagesTotal( summary.packagesToGetAndInstall() );
	RuntimeData::instance()->setCommitPackagesTotal( s_zypp->resolver()->getTransaction().actionSize() ); //temporary fix
	RuntimeData::instance()->setCommitPackageCurrent( 0 );
	RuntimeData::instance()->setRpmPackagesTotal( s_zypp->resolver()->getTransaction().actionSize() );
	RuntimeData::instance()->setRpmPackageCurrent( 0 );
	RuntimeData::instance()->setEnteredCommit( true );
	
	ZYppCommitResult result = s_zypp->commit( policy );
	
	RuntimeData::instance()->setEnteredCommit( false );
	if ( !result.allDone() || !( dryRunFlag && result.noError() ) ) {
	     //OCIHelper::instance()->setExitCode( result.attemptToModify() ?
		//				 ERR_COMMIT : ERR_ZYPP );
	    throw "Incomplete Commit";
	}
	cout << "Commit Succeeded" << endl;
    }
    catch ( const media::MediaException& e)
    {
	string errDescription = "Problem retrieving the package file from the repository";
	//OCIHelper::instance()->setErrorDescription( ERR_COMMIT, errDescription );
	return false;
    }
    catch ( repo::RepoException& e )
    {
	bool refreshNeeded = false;
	try {
	    RepoManager manager( Pathname( "/" ) );
	    for ( RepoInfo::urls_const_iterator it = e.info().baseUrlsBegin();
		 it != e.info().baseUrlsEnd();
		 ++it )
	    {
		if ( manager.checkIfToRefreshMetadata( e.info(), *it, RepoManager::RefreshForced ) == RepoManager::REFRESH_NEEDED ) 
		{
		    refreshNeeded = true;
		    break;
		}
	    }
	}
	catch( const Exception& e )
	{ cout << "Ingore - This is just check if to refresh exception" << endl; }
	
	string message;
	if ( refreshNeeded ) // this exception is highly unlikely as we are refreshing all the enabled repos during installation
	    message = str::Format( "Repository '%s' is out of date. Running '%s' might help" ) % e.info().alias() % "zypper refresh";
	else
	    message = "Problem retrieving the package file from the repository";
	//OCIHelper::instance()->setExitDescription( ERR_COMMIT, message );
	return false;
    }
    catch ( const FileCheckException& e)
    {
	string message = "The package integrity check failed. This may be a problem"
			  " with the repository or media. Please try one of the following:\n"
			  "- refresh the repositories using 'zypper refresh'\n"
			  "- use another installation medium (if damaged)\n"
			  "- use another repository.\n"
			  "Sorry to disappoint you! This is out of the scope of OCI (for now?)";
	//OCIHelper::instance()->setExitDescription( ERR_COMMIT, message );
	return false;
    }
    catch ( const Exception& e )
    {
	string message = "Problem occured during or after installation or removal of packages";
	//OCIHelper::instance()->setExitDescription( ERR_COMMIT, message );
	return false;
    }
    catch ( const char* exp)
    {
	cout << "Error committing";
    }
    
    /*string output; 
    if ( OCIHelper::instance()->exitCode() != ZYPP_OK ) {
	output = "Installation has completed with error. ";
	if ( OCIHelper::instance()->exitCode() == ERR_COMMIT )
	    output.append( "Please run 'zypper verify' to repair any dependency problem." );
	// send output to OCI
    }*/
    return true;
}

void ZyppInstall::initTarget( const Pathname& sysRoot )
{
    cout << "Initializing target at " << sysRoot << endl;
    s_zypp->initializeTarget( sysRoot ); //initialize target
    cout << "Loading target resolvables" << endl;
    s_zypp->getTarget()->load(); // load installed packages to pool
}

ZYpp::Ptr ZyppInstall::zyppPtr()
{
    return s_zypp;
}
