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

#include <klocalizedstring.h>
#include "mainheader.h"

MainHeader::MainHeader( QWidget *parent )
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    setStyleSheet( "background-color : rgb(251,248,241)" );
    m_statusLabel = new QLabel( i18n("This installer will install and download packages") );

    m_statusLabel->setWordWrap( true );
    m_statusLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    const QPixmap icon( "/usr/share/icons/hicolor/32x32/apps/oneclickinstall.png" );
    m_icon = new QLabel;
    m_icon->setPixmap( icon );

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget( m_icon );
    mainLayout->setSpacing( 20 );
    mainLayout->addWidget( m_statusLabel );

    setLayout( mainLayout );
    show();
}

void MainHeader::changeStatusLabel( int repoCount, int packageCount )
{
    m_packageCount = packageCount;
    m_repoCount = repoCount;

    m_statusLabel->setText( i18np("This installer will download and install %1 package from %2 source", "This installer will download and install %1 packages from %2 sources", packageCount, repoCount) );
}

void MainHeader::updateDetails( const QString& size )
{
    m_totalSize += size.split(" ").at(0).toFloat();
    QString sizeString = (QString::number(m_totalSize)).append(size.split(" ").at(1));
    m_statusLabel->setText( i18ncp( "Third argument is the total size of all packages", "This installer will download and install %1 package from %2 source totalling %3", "This installer will download and install %1 packages from %2 source(s) totalling %3", m_packageCount, m_repoCount, sizeString) );
}

void MainHeader::showCheckForConflictsHeader()
{
    m_statusLabel->setText( i18n( "Checking for <b>Software Conflicts</b> and <b>Package Dependencies</b>. Please Wait" ) );
}

void MainHeader::installationStarted()
{
    m_statusLabel->setText( i18ncp("Decides which sentence to use based on number of packages","Downloading and Installing Package...", "Downloading and Installing Packages...", m_packageCount) );
}

void MainHeader::installationCompleted()
{
    m_statusLabel->setText( i18n("Success!!!") );
}
