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
#include "utils.h"

PackageMetadata::PackageMetadata()
{   
}

void PackageMetadata::getData( const QString& packageName )
{
    PoolItem packageObj = ZypperUtils::queryMetadataForPackage(packageName.toStdString());
    if (!packageObj) {
      qDebug() << "Package Not found";
      // Will think of a way how to make user known about this
      return;
    }
    m_version = QString::fromStdString(packageObj.edition().asString());
    m_size = QString::fromStdString(packageObj.installSize().asString());
    
    //invoke isFinished() slot manually
    QMetaObject::invokeMethod(this, "isFinished", Qt::QueuedConnection);
}

QString PackageMetadata::size()
{
    return m_size;
}

QString PackageMetadata::version()
{
    return m_version;
}

void PackageMetadata::isFinished()
{
    qDebug() << m_size;
    qDebug() << m_version;

    emit finished( m_version, m_size );
}
