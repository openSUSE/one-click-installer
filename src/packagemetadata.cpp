//      Copyright 2012 Saurabh Sood <saurabh@saurabh.geeko>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//
//

#include "packagemetadata.h"

PackageMetadata::PackageMetadata( QString name )
{
    m_package = name;
}

void PackageMetadata::getData()
{
    QString processName = "zypper info " + m_package;
    qDebug() << processName;
    m_process = new QProcess;
    m_process->setProcessChannelMode( QProcess::MergedChannels );
    QObject::connect( m_process, SIGNAL( finished( int ) ), this, SLOT( dataChanged( int ) ) );
    QObject::connect( m_process, SIGNAL( started() ), this, SLOT( isStarted() ) );
    m_process->start( processName );
}

QString PackageMetadata::size()
{
    return m_size;
}

QString PackageMetadata::version()
{
    return m_version;
}

void PackageMetadata::isStarted()
{
    qDebug() << "zypper called";
}

void PackageMetadata::dataChanged( int v )
{
    m_stdout = m_process->readAllStandardOutput();
    qDebug() << "finished process";
    QStringList details = m_stdout.split( "\n" );

    foreach( QString line, details ) {
        if( line.contains("Version") ) {
            m_version = line.split( " " ).at( 1 );
        } else if( line.contains( "Installed Size" ) ) {
            QStringList size = line.split( " " );
            m_size = size.at( 2 ) + " " + size.at( 3 );
        }
    }

    qDebug() << m_size;
    qDebug() << m_version;

    emit data( m_version, m_size );
}
