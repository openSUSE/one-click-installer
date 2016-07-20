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

#include "backendoci.h"
#include "utils.h"

BackendOCI::BackendOCI(QString tempFileName, int ociID)
{
    m_tempFileName = tempFileName;
    m_ociID = ociID;
}

bool BackendOCI::exists(const QString& repoUrl)
{
    return ZypperUtils::exists( repoUrl.toStdString() );
}

void BackendOCI::callBackendHelper()
{
    m_process = new QProcess;
    
    QString command("xdg-su -u root -c \"/usr/sbin/oneclickhelper ");
    command.append( m_tempFileName );
    //command.append( QString::number( m_ociID ) ); Not needed :)
    command.append( "\"" );
    qDebug() << command;
    
    m_process->start( command );
    emit checkForConflicts();	// Gui while OCIhelper checks for conflicts
}
