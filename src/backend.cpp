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

// static variable
QStringList Backend::s_packages;

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
    
    install();
}

void Backend::install() 
{
    /******************************************* INSTALLATION *****************************************/
    // Step 1. Mark packages for installation
    
    if ( s_packages.isEmpty() )
	return;
    
    QString package = s_packages.takeFirst();
    ZyppInstall::markPackagesForInstallation( package.toStdString() );
    m_zypp = ZyppInstall::zyppPtr();
    
    // Step 2. Resolve package dependencies
    resolveConflicts();
}

void Backend::resolveConflicts()
{
    if ( !ZyppInstall::resolveConflictsAndDependencies() ) {
	emit hasConflicts();
	qDebug() << "emitting hasConflicts!";
	resolve();
	return;
    }
    
    // This is the point where there are no conflicts, or all conflicts have been solved
    emit noConflicts();
    
    // Step 3. Commit Changes
    ZyppInstall::commitChanges();
    
    // Step 4. Continue the process, if there are more than 1 packages to install 
    install();
}

void Backend::resolve()
{
    m_resolverProblemList = QList<ResolverProblem_Ptr>::fromStdList( m_zypp->resolver()->problems() );
    qDebug() << "size : " << m_resolverProblemList.size();
    m_solutionsToTry.clear();
    resolve( *m_resolverProblemList.takeFirst() );
}

void Backend::resolve( const ResolverProblem& problem )
{
    m_currentProblem = problem;
    QString problemStatement( "<b>Problem:</b> " + QString::fromStdString( problem.description() ) );
    QStringList solProposals;
    
    const ProblemSolutionList & solutions = problem.solutions();
    for ( const auto & solPtr : solutions ) {
	solProposals.append( QString::fromStdString( solPtr->description() ) );
    }
    
    // Emit signal displayProblemSolution( QString, QStringList ) - First string = problem, second stringlist = solutions
    emit displayProblemAndSolutions( problemStatement, solProposals );
}

void Backend::applySolution( int solId )
{
    const ProblemSolutionList & solutions = m_currentProblem.solutions();
    ProblemSolutionList::const_iterator solIter = solutions.begin();
    std::advance( solIter, solId );
    m_solutionsToTry.append( *solIter );
    
    qDebug() << "Returned solId from OCI - " << solId;
    qDebug() << "===== Selected solution =====";
    qDebug() << QString::fromStdString( ( *solIter )->description() );
    qDebug() << "=============================";
	    
    if ( !m_resolverProblemList.isEmpty() )
	resolve( *m_resolverProblemList.takeFirst() );
    else {
	qDebug() << "Apply selected solutions...";
	m_zypp->resolver()->applySolutions( m_solutionsToTry.toStdList() );
	qDebug() << "Dependencies solved";
	resolveConflicts();
    }
}

void Backend::addPackage(const QString& package)
{
    s_packages << package;
}

void Backend::killBackend()
{
    QCoreApplication::quit();
}
