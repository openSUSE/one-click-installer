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
#include <QCoreApplication>
#include <QDebug>
#include <QDBusConnection>
#include "backend.h"
#include "ocihelperadaptor.h"	//generated during build time

Backend::Backend()
{	
    new OCIHelperAdaptor( this );
    QDBusConnection connection = QDBusConnection::systemBus();
    if ( !connection.isConnected() ) {
	qFatal( "Cannot connect to the D-Bus system bus" );
	exit( 1 );
    }
    connection.registerObject( "/", this );
    if ( !connection.registerService( "org.opensuse.OCIHelper" ) ) {
	qDebug() << qPrintable( connection.lastError().message() );
	exit( 1 );
    }

    // Proxy for OCI
    QDBusConnection sysBus = QDBusConnection::systemBus();
    if ( !sysBus.isConnected() ) {
	qFatal( "Cannot connect to the D-Bus system bus" );
	exit( 1 );
    }
    m_oci = new org::opensuse::oneclickinstaller( "org.opensuse.oneclickinstaller", "/OCI", sysBus, this );
    if ( !m_oci->isValid() ) {
	qFatal( "Oops! Cannot connect to the service org.opensuse.oneclickinstaller" );
	exit( 1 );
    }
    
    /******************************************* INSTALLATION *******************************************/
    // Step 1. Mark packages for installation
    ZypperUtils::markPackagesForInstallation( "http://download.videolan.org/SuSE/Tumbleweed/", "vlc" );
    
    // Step 2. Resolve package dependencies
    while ( !ZypperUtils::resolveConflictsAndDependencies() ) {
	emit hasConflicts();
	qDebug() << "emitting hasConflicts!";
	resolve();
    }
    // This is the point where there are no conflicts, or all conflicts have been solved
    emit noConflicts();
    
    // Step 3. Commit Changes
    ZypperUtils::commitChanges();
    /***************************************************************************************************/
}

void Backend::resolve( const ResolverProblem& problem, ProblemSolutionList& todo )
{
    QString problemStatement( "<b>Problem:</b> " + QString::fromStdString( problem.description() ) );
    QString solProposals("");
    
    const ProblemSolutionList & solutions = problem.solutions();
    for ( const auto & solPtr : solutions ) {
	// "$" is the separator
	solProposals.append( QString::fromStdString( solPtr->description() ) + "$" );
    }
    
    // Emit signal displayProblemSolution( QString, QString ) - First string = problem, second string = solutions
    // Each solution is separated by a "$"
    emit displayProblemAndSolutions( problemStatement, solProposals );
    
    // get the solutionID here
    int solId;
    while ( ( solId = m_oci->solutionID() ) == -1 );
    
    ProblemSolutionList::const_iterator solIter = solutions.begin();
    std::advance( solIter, solId );
    
    qDebug() << "Returned solId from OCI - " << solId;
    qDebug() << "===== Selected solution =====";
    qDebug() << QString::fromStdString( ( *solIter )->description() );
    qDebug() << "=============================";
	    
    todo.push_back( *solIter );
}

void Backend::resolve()
{
    ZYpp::Ptr zypp = ZypperUtils::zyppPtr();
    
    const ResolverProblemList & problems( zypp->resolver()->problems() );
    ProblemSolutionList toTry;
    
    // list all the problems with solutions on the GUI
    for ( const auto & probPtr : problems ) {
	resolve( *probPtr, toTry );
    }
    
    //apply solutions...
    if ( !toTry.empty() ) {
	qDebug() << "Apply selected solutions...";
	zypp->resolver()->applySolutions( toTry );
	qDebug() << "Dependencies solved";
	return;
    }
    else 
	throw "Solving dependencies failed: Giving up!";
}

void Backend::install() {}

void Backend::addRepository() {}

void Backend::addPackage() {}

void Backend::killBackend()
{
    QCoreApplication::quit();
}
