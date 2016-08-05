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
 *  Previous Contributor: Saurabh Sood
 ***********************************************************************************/

#include "packagemetadata.h"
#include "zyppinfo.h"

PackageMetadata::PackageMetadata()
{   
}

void PackageMetadata::getData( const QString& packageName )
{
    Info packageInfo = ZyppInfo::queryMetadataForPackage( packageName.toStdString() );
    
    m_version = QString::fromStdString( packageInfo.version() );
    m_size = QString::fromStdString( packageInfo.installedSizeAsString() );
    
    // We use QMetaObject::invokeMethod to queue the call until the finished() signal is connected
    // by the parent object later on in the code. Please, don't replace it with a direct call to
    // sizeAndVersionObtained unless you really know what you're doing.
    QMetaObject::invokeMethod(this, "sizeAndVersionObtained", Qt::QueuedConnection);
}

QString PackageMetadata::size()
{
    return m_size;
}

QString PackageMetadata::version()
{
    return m_version;
}

void PackageMetadata::sizeAndVersionObtained()
{
    qDebug() << m_size;
    qDebug() << m_version;

    emit finished( m_version, m_size );
}
