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
 *  Previous Contributor(s): Saurabh Sood 
 ***********************************************************************************/

#include <iostream>
#include <QDebug>
#include <QDBusConnection>
#include "backend.h"
#include "ocihelperadaptor.h"	//generated during build time

using namespace std;

Backend::Backend()
{	
    new OCIHelperAdaptor( this );
    QDBusConnection connection = QDBusConnection::systemBus();
    connection.registerObject( "/", this );
    connection.registerService( "org.opensuse.OCIHelper" );
    
    // Proxy for OCI
    QDBusConnection sysBus = QDBusConnection::systemBus();
    m_oci = new org::opensuse::oneclickinstaller("org.opensuse.oneclickinstaller", "/OCI", sysBus, this);
        
    // Emit signal displayProblemSolution( QString, QString ) - First string = problem, second string = solutions
    // Each solution is separated by a "/"
    emit hasConflicts();
    emit displayProblemAndSolutions("Hello", "sfifdsjfdsihsdvsjvsihvsdjvsji/fsdlhfsdlifsdlisdfdf/dfshufsdlihsdflhf");
    
    //sysBus.connect( QString(), QString(), "org.opensuse.oneclickinstaller", "solutionNumber", this, SLOT() );
}

void Backend::install() {}

void Backend::addRepository() {}

void Backend::addPackage() {}

void Backend::killBackend()
{
    QCoreApplication::quit();
}
