/***********************************************************************************
 *  One Click Installer makes it easy for users to install software, no matter
 *  where that software is located.
 *
 *  Copyright (C) 2016  Shalom Rachapudi David <shalomray7@gmail.com>
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

#include <QHBoxLayout>
#include <QTextStream>
#include <QGridLayout>
#include <QIcon>
#include <klocalizedstring.h>
#include "finallogscreen.h"
#include "installscreen.h"
#include "oci_helper_interface.h"

FinalLogScreen::FinalLogScreen()
{
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setSpacing( 0 );
  
    m_packageInstalled = true;
    m_numOfPackagesInstalled = 0;
    
    setStyleSheet( "background-color: white;" );
    
    setLayout( m_mainLayout );
}

void FinalLogScreen::processInstalledPackages( const QStringList & installationLog, bool commitStatus )
{
    QWidget *statusWidget = new QWidget;
    statusWidget->setObjectName( "statusWidget" );
    statusWidget->setStyleSheet( "QWidget#statusWidget{ background-color : white; border-bottom: 1px solid rgb(196, 181, 147); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); }" );
    
    QVBoxLayout *statusLayout = new QVBoxLayout( statusWidget );
    
    for ( QString line : installationLog ) {
        if ( line.at( 0 ) == 'N' )
	    m_mainLayout->addWidget( repoWidget( line ) );
        else 
	    statusLayout->addLayout( installedWidget( line ) );
    }
    
    /*
     * emit showFinalInstallationLogHeader( bool commitStatus, bool packageInstalled ) signal to MainHeader
     * There's always a case where few packages do not get installed correctly. How do we differentiate this scenario form
     * all packages do not get installed? This is where packageInstalled flag comes into play.
     * 
     * commitStatus = false; [and] packageInstalled = true; (true by default) -> No packages are installed
     * commitStatus = false; [and] packageInstalled = false; -> Atleast one package is not installed correctly
     */
     emit finalLogHeader( m_numOfPackagesInstalled, commitStatus, m_packageInstalled ); 
     
    // add statusWidget to the m_mainLayout
    m_mainLayout->addWidget( statusWidget );
}

QLabel* FinalLogScreen::repoWidget( const QString & line )
{
    QString description = i18n( "Added source: <b>%1</b> " ).arg( line.split( " " ).at( 1 ) );
    qDebug() << description;
    
    QLabel *repoLabel = new QLabel( description );
    repoLabel->setStyleSheet( "background-color: rgb(254, 250, 210); padding: 5px; border-bottom : 1px solid rgb(252,233,79); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147);" );
    repoLabel->setMinimumHeight( 35 );
    repoLabel->setWordWrap( true );
    
    return repoLabel;
}

QPixmap FinalLogScreen::image( bool success )
{   
    return QIcon::fromTheme( success ? "okIcon" : "errorInstall" ).pixmap( 22, 22 );
}

QGridLayout* FinalLogScreen::installedWidget( const QString & packageName )
{
    QGridLayout *layout = new QGridLayout();
    layout->setColumnStretch(1, 30);
    bool success = (bool) ( packageName.split( " " ).at( 1 ) ).toInt();
    
    if ( !success )
	m_packageInstalled = false;
    else
	++m_numOfPackagesInstalled;
    
    QLabel *package = new QLabel( packageName.split( " " ).at( 0 ) );
    package->setStyleSheet( "padding: 3px;" );
       
    // add package, and status widget to Grid Layout
    layout->addWidget( package, 0, 1 );
    layout->addLayout( packageStatus( success ), 0, 2 );
    
    return layout;
}

QHBoxLayout* FinalLogScreen::packageStatus( bool success )
{
    QHBoxLayout *layout = new QHBoxLayout();
    
    QLabel *statusIcon = new QLabel();
    QLabel *statusInfo = new QLabel();
    
    // stylesheet 
    statusIcon->setStyleSheet( "padding: 3px;" );
    statusInfo->setStyleSheet( "padding: 3px;" );
    
    statusIcon->setPixmap( QPixmap( image( success ) ) );
    statusInfo->setText( success ? "Installed" : "Error Installing" );
    
    layout->addWidget( statusIcon );
    layout->addWidget( statusInfo );
    
    return layout;
}

void FinalLogScreen::close()
{
    org::opensuse::OCIHelper *m_ociHelper = new org::opensuse::OCIHelper("org.opensuse.OCIHelper", "/", QDBusConnection::systemBus(), this);
    if ( !m_ociHelper->isValid() ) {
	qDebug() << "Oops! Cannot connect to the service org.opensuse.OCIHelper";
    }
    m_logFile.close();
    m_ociHelper->killBackend();
    qApp->quit();
}
