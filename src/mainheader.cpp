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


#include "mainheader.h"

MainHeader::MainHeader( QWidget *parent )
{
    setStyleSheet( "background-color : rgb(251,248,241)" );
    m_statusLabel = new QLabel( "This installer will install and download packages" );

//    m_statusLabel->setWordWrap( true );

    const QPixmap icon( "/usr/share/icons/oneclickinstall.png" );
    m_icon = new QLabel;
    m_icon->setPixmap( icon );

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget( m_icon );
    mainLayout->setSpacing( 0 );
    mainLayout->addWidget( m_statusLabel );

    setLayout( mainLayout );
    show();
}

void MainHeader::changeStatusLabel( int repoCount, int packageCount )
{
    m_packageCount = packageCount;
    m_repoCount = repoCount;

    m_statusLabel->setText( QString( "This installer will download and install %1 packages from %2 sources" ).arg( packageCount ).arg( repoCount ) );
}

void MainHeader::updateDetails( QString size )
{
    m_statusLabel->setText( QString( "This installer will download and install %1 packages from %2 sources totalling %3" ).arg( m_packageCount ).arg( m_repoCount ).arg( size ) );
}
