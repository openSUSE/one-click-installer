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


#include "installscreen.h"

InstallScreen::InstallScreen(PackageBackend *backend, QString *tmpFileName, QObject *parent )
{
    setStyleSheet( "" );
    m_backend = backend;
    m_tmpFileName = tmpFileName;

    QWidget *packageWidget = new QWidget;
    packageWidget->setObjectName( "packageWidget" );
    packageWidget->setStyleSheet( "QWidget#packageWidget{ background-color : white; border-bottom : 1px solid rgb(196,181,147); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); }" );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVBoxLayout *installLayout = new QVBoxLayout( packageWidget );

    QVBoxLayout *sourceLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    m_installStatus = new QLabel( "Downloading and Installing Packages" );
    m_cancel = new QPushButton( "Cancel Installation" );

    foreach( QUrl iter, m_backend->repositories() ) {
        QLabel *sourceLabel = new QLabel( QString( "Added Source: %1" ).arg( iter.toString() ) );
        sourceLabel->setStyleSheet( "background-color: rgb(254, 250, 210); border-bottom : 1px solid rgb(252,233,79); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147);" );
        sourceLayout->addWidget( sourceLabel );
        sourceLayout->setSpacing( 0 );
        sourceLabel->setFixedHeight( 40 );
//        sourceLabel->setContentsMargins( 20, 20, 20, 20 );
    }

    foreach( QString iter, m_backend->packages() ) {
        QHBoxLayout *packageLayout = new QHBoxLayout;
        QLabel *package = new QLabel( QString( "<b>Installing: </b> %1" ).arg( iter ) );
        package->setFixedHeight( 40 );
        package->setStyleSheet( "background-color : white" );
        QProgressBar * progressBar = new QProgressBar;
        progressBar->setFixedHeight( 20 );
        progressBar->setMinimum( 0 );
        progressBar->setMaximum( 0 );
        progressBar->setRange( 0, 0 );
        packageLayout->addWidget( package );
        packageLayout->setSpacing( 200 );
        packageLayout->addWidget( progressBar );
        installLayout->addLayout( packageLayout );
    }

    buttonLayout->addSpacing( 400 );
    buttonLayout->addWidget( m_cancel );
    mainLayout->setSpacing( 0 );
    mainLayout->addLayout( sourceLayout );
    mainLayout->addSpacing( 0 );
    mainLayout->addWidget( packageWidget );
    mainLayout->addSpacing( 20 );
    mainLayout->addLayout( buttonLayout );

    setLayout( mainLayout );
}

void InstallScreen::showEvent( QShowEvent *s )
{
    QTimer::singleShot( 0, this, SLOT( callBackend() ) );
}

void InstallScreen::callBackend()
{
    m_backend->setFileName( *m_tmpFileName );
    m_backend->callBackendHelper();
}
