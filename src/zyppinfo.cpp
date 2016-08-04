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

#include <zypp/ByteCount.h>
#include "zyppinfo.h"

Info ZyppInfo::queryMetadataForPackage( const string& packageName )
{
    PoolQuery q;
    q.addKind( ResKind::package );
    q.addAttribute( sat::SolvAttr::name, packageName );
    q.setMatchExact();

    return packageObject( q );
}

Info ZyppInfo::packageObject( const PoolQuery& q )
{
    Info packageInfo;
    for_( it, q.selectableBegin(), q.selectableEnd() ) {
	const ui::Selectable& s =  *(*it);   
	PoolItem installedObject( s.installedObj() );
	// An update candidate object is better than any installed object
	PoolItem updateObject( s.updateCandidateObj() );
	
	/* What if the installed version is the best (latest) version?
	 * "updateObject.repository()" would fail (because there is no update candidate;
	 * so does installedObject.repository() if there isn't one. Following code takes care of that
	 */
	PoolItem mainObject( updateObject );
	if ( !mainObject ) {
	    mainObject = s.identicalAvailableObj( installedObject );
	    if ( !mainObject )
		mainObject = installedObject;
	}
	
	// Fill in the information
	// mainObject.repository() wouldn't fail in any scenario/case
	packageInfo.setRepositoryName( mainObject.repository().asUserString() );
	packageInfo.setPackageName( mainObject.name() );
	packageInfo.setVersion( mainObject.edition().asString() );
	packageInfo.setArch( mainObject.arch().asString() );
	packageInfo.setVendor( mainObject.vendor().asString() );
	packageInfo.setInstalled( (bool)installedObject );
	
	// set InstalledSize
	ByteCount count = mainObject.installSize();
	packageInfo.setInstalledSize( (unsigned long long)count );
	packageInfo.setInstalledSizeStr( count.asString() );
	
	// set status
	string status;
	stringstream format;
	if ( !installedObject )
	    status = "Not Installed";
	else {
	    if ( updateObject ) {
		format << "Archaic (version " << installedObject.edition().asString() << " installed)";
		status = format.str();
	    }
	    else 
		status = "Up to Date";
	}
	packageInfo.setStatus( status );
    }
    return packageInfo;
}

KeyRingReceive ZyppInfo::keyReport()
{
    return ZypperUtils::keyReport();
}
