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
#include "zyppinstall.h"
#include "utils.h"

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
