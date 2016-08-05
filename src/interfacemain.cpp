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
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QUrl>
#include <QStringList>
#include "backend.h"
#include "utils.h"

int main( int argc, char *argv[] )
{
    QCoreApplication app ( argc, argv );
    qDebug() << "Helper Started";
    if ( argc < 2 ) {
	qDebug() << "Usage: ./oneclickhelper <File Path> <Repository Name>";
	return 1;
    }
    
    // Reset RepoManager. This step is necessary to run the manager with root privileges
    ZypperUtils::resetRepoManager( Pathname( "/" ) );
    
    QFile dataFile( argv[ 1 ] );
    if( !dataFile.open( QIODevice::ReadOnly ) ) {
        qDebug() << "Failed to open Data File";
        return 1;
    }
    QTextStream inData( &dataFile );

    /* Process the file and add repos to the system and store packages in a QList */ 
    QString repoAlias;
    QString line;
    do {
        line = inData.readLine();
        if( !line.isNull() ) {
	    if (line.at( 0 ) == 'N' ) {
		repoAlias = line.split( " " ).at( 1 );
		repoAlias.append( "-repo" );
	    } else if( line.at( 0 ) == 'R' ) {
                QString repo = line.split( " " ).at( 1 );
                qDebug() << repo;
		ZypperUtils::addRepository( repo.toStdString(), repoAlias.toStdString() );  // Add repo listed in .ymp file to the system repos
            } else if( line.at( 0 ) == 'P' ) {
                QString package = line.split( " " ).at( 1 );
                qDebug() << package;
                Backend::addPackage( package );
            }
        }
    } while( !line.isNull() );
    
    Backend *ptr = new Backend();
    
    return app.exec();
}
