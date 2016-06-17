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
 ***********************************************************************************/


#include <zypp/ResKind.h>
#include <zypp/base/Algorithm.h>
#include <zypp/ZYpp.h>
#include "utils.h"

/************************************* ZypperUtils Class **************************************/
// Define static variables here
RepoManagerOptions* ZypperUtils::s_repoManagerOpts = new RepoManagerOptions("/tmp");
RepoManager* ZypperUtils::s_repoManager = new RepoManager(*s_repoManagerOpts);
PoolItem ZypperUtils::mainObject;
KeyRingReceive ZypperUtils::s_keyReceiveReport;

//Methods

void ZypperUtils::initRepository( const string& repoUrl )
{
    //KeyRing - Handling of gpg keys
    s_keyReceiveReport.connect();
    refreshRepoManager( initRepoInfo( repoUrl ) );
    s_keyReceiveReport.disconnect();
    RepoInfo info = s_repoManager->getRepo( repoUrl );
    cout << "===================================================" << endl;
    cout << "Alias: " << info.alias() << endl;
    cout << "Url: " << info.url() << endl;
    cout << "===================================================" << endl;
}

RepoInfo ZypperUtils::initRepoInfo( const string& url )
{
    RepoInfo repoInfo;
    //Url repoUrl(url);
    repoInfo.addBaseUrl( Url( url ) );
    repoInfo.setAlias( url );
    repoInfo.setGpgCheck( true );
    repoInfo.setAutorefresh( true );
  
    // TRUST_KEY_TEMPORARILY because it is for temporary use
    KeyRing::setDefaultAccept( KeyRing::TRUST_KEY_TEMPORARILY );
    cout << "TRUST_KEY_TEMPORARILY accepted" << endl;
    // Add it to the repoManager if not already present
    if ( !s_repoManager->hasRepo( url ) ) {
	s_repoManager->addRepository(repoInfo);
	cout << "added repo" << endl;
    }
    // Return repoInfo to refresh the repo (cache)
    return repoInfo;
}
 
void ZypperUtils::refreshRepoManager( const RepoInfo& temp ) 
{
    s_repoManager->refreshMetadata( temp, RepoManager::RefreshForced );
    s_repoManager->buildCache( temp );
    s_repoManager->loadFromCache( temp ); 
}

PoolItem ZypperUtils::queryMetadataForPackage( const string& packageName )
{
    PoolQuery q;
    q.addKind( ResKind::package );
    q.addAttribute( sat::SolvAttr::name, packageName );
    q.setMatchExact();

    return packageObject( q );
}
  
PoolItem ZypperUtils::packageObject(const PoolQuery& q )
{
    for( PoolQuery::Selectable_iterator it = q.selectableBegin(); it != q.selectableEnd(); ++it ) {
	const ui::Selectable& s =  *(*it);   
	PoolItem installedObject( s.installedObj() );
	// An update candidate object is better than any installed object
	PoolItem updateObject( s.updateCandidateObj() );
	PoolItem mainObjectTemp( updateObject );
	if ( !mainObjectTemp )
	    mainObject = installedObject;
	else
	    mainObject = mainObjectTemp;
    }
    return mainObject;
}

KeyRingReceive ZypperUtils::keyReport()
{
    return s_keyReceiveReport;
}

