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

    m_watcher = new QFileSystemWatcher;
    m_watcher->addPath( QString::fromLocal8Bit( "/var/log/oneclick.log" ) );

    QObject::connect( m_watcher, SIGNAL( fileChanged( QString ) ), this, SLOT( logFileChanged( QString ) ) );

    QWidget *packageWidget = new QWidget;
    packageWidget->setObjectName( "packageWidget" );
    packageWidget->setStyleSheet( "QWidget#packageWidget{ background-color : white; border-bottom : 1px solid rgb(196,181,147); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); }" );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVBoxLayout *installLayout = new QVBoxLayout( packageWidget );

    QVBoxLayout *sourceLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    m_installStatus = new QLabel( "Downloading and Installing Packages" );
    m_cancel = new QPushButton( "Cancel Installation" );

    QObject::connect( m_cancel, SIGNAL( clicked() ), this, SLOT( cancelInstallation() ) );

    m_progressBar = new QProgressBar;
    m_progressBar->setFixedHeight( 20 );
    m_progressBar->setMinimum( 0 );
    m_progressBar->setMaximum( 100 );
    m_progressBar->setRange( 0, 100 );

    installLayout->addWidget( m_progressBar );
    installLayout->setSpacing( 20 );

    foreach( QUrl iter, m_backend->repositories() ) {
        QLabel *sourceLabel = new QLabel( QString( "Added Source: %1" ).arg( iter.toString() ) );
        sourceLabel->setStyleSheet( "background-color: rgb(254, 250, 210); border-bottom : 1px solid rgb(252,233,79); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147);" );
        sourceLayout->addWidget( sourceLabel );
        sourceLayout->setSpacing( 0 );
        sourceLabel->setFixedHeight( 40 );
//        sourceLabel->setContentsMargins( 20, 20, 20, 20 );
    }

    int i = 0;

    foreach( QString iter, m_backend->packages() ) {
        QHBoxLayout *packageLayout = new QHBoxLayout;
        m_packageLayouts.insert( i, packageLayout );
        QLabel *package = new QLabel( QString( "<b>Installing: </b> %1" ).arg( iter ) );
        package->setFixedHeight( 40 );
        package->setStyleSheet( "background-color : white" );
        packageLayout->addWidget( package );
        packageLayout->setSpacing( 200 );
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


void InstallScreen::showCompletionStatus()
{
    m_progressBar->hide();

    foreach( QHBoxLayout *l, m_packageLayouts ) {
        QLabel *completed = new QLabel( "Installed" );
        completed->setStyleSheet( "background-color : white" );
        l->addWidget( completed );
    }
}

void InstallScreen::logFileChanged( QString path )
{
    qDebug() << "changed";
    QFile file( path );
    if( file.open( QIODevice::ReadOnly ) ) {
        QString str( file.readAll() );

        if( str == "" || str.isNull() )
            return;
        QStringList line = str.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

        QString final = line.last();
        qDebug() << final;
        QString num = final.split( "=" ).at( 1 );
        num.remove( 0, 1 );
        num.remove( num.length() - 1, 1 );
        int val = num.toInt();

        qDebug() << val;

        m_progressBar->setValue( val );

    }
}

void InstallScreen::cancelInstallation()
{
    qDebug() << "cancelling installation";

    ClientDBus *client = new ClientDBus( "org.opensuse.oneclickinstaller", "/", QDBusConnection::sessionBus(), 0 );
    client->KillBackend();
    qApp->quit();
}
