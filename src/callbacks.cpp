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

#include "callbacks.h"
#include "mediacallbacksadaptor.h"

Callbacks::Callbacks()
{
    new MediaCallbacksAdaptor( this );
    QDBusConnection connection = QDBusConnection::systemBus();
    if ( !connection.isConnected() ) {
	qFatal( "Cannot connect to the D-Bus system bus" );
	exit( 1 );
    }
    connection.registerObject( "/Media", this );
    if ( !connection.registerService( "org.opensuse.MediaCallbacks" ) ) {
	qDebug() << qPrintable( connection.lastError().message() );
	exit( 1 );
    }
}

void Callbacks::emitStartResolvable( QString info )
{ emit startResolvable( info ); }

void Callbacks::emitFinishResolvable( bool success )
{ emit finishResolvable( success ); }

void Callbacks::emitProgress( int value )
{ emit progress( value ); }
