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

#include <klocalizedstring.h>
#include <QHBoxLayout>
#include "installscreen.h"

InstallScreen::InstallScreen()
{
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setSpacing( 0 );
    m_currentPackage = 0;
        
    // log file
    m_logFile.setFileName( "/tmp/oci_log.txt" );
    if( !m_logFile.open( QIODevice::Truncate | QIODevice::WriteOnly ) ) {
        qDebug() << "Could not open Data File";
    }
    m_outData = new QTextStream( &m_logFile );
    
    // status widget and its layout
    m_statusWidget = new QTextBrowser;
    m_statusWidget->setObjectName( "statusWidget" );
    m_statusWidget->setStyleSheet( "QTextBrowser#statusWidget{ background-color : white; border-bottom: 1px solid rgb(196, 181, 147); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); }" );
    m_statusWidget->setMinimumSize( 500, 180 );
       
    // Cancel button
    m_cancelButton = new QPushButton( i18n( "Cancel" ) );
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addSpacing( 400 );
    buttonLayout->addWidget( m_cancelButton );
    
    // Current message label, Progress Bar
    m_currentPackageStatusLabel = new QLabel( i18n( "Please Wait..." ) );
    m_progressBar = new QProgressBar;
    m_progressBar->setMinimum( 0 );
    m_progressBar->setMaximum( 100 );
    m_progressBar->setRange( 0, 100 );
    m_progressBar->setTextVisible( true );
    
    // total progress message label, Total Progress Bar
    m_totProgBarStatusLabel = new QLabel( i18n( "Total Progress:" ) );
    m_totalProgressBar = new QProgressBar;
    m_totalProgressBar->setMinimum( 0 );
    m_totalProgressBar->setRange( 0, 100 );
    m_totalProgressBar->setTextVisible( true );
    
    // All set! Now add widgets to the m_mainLayout
    m_mainLayout->addWidget( m_statusWidget );
    m_mainLayout->addWidget( horizontalLine() );
    m_mainLayout->addWidget( m_currentPackageStatusLabel );
    m_mainLayout->addWidget( m_progressBar );
    m_mainLayout->addWidget( m_totProgBarStatusLabel );
    m_mainLayout->addWidget( m_totalProgressBar );
    m_mainLayout->addWidget( horizontalLine() );
    m_mainLayout->addLayout( buttonLayout );
    
    // Signals and slots
    QObject::connect( m_cancelButton, SIGNAL( clicked() ), this, SLOT( cancelInstallation() ) );
    
    setLayout( m_mainLayout );
}

void InstallScreen::initDBusServices()
{
    m_ociHelper = new org::opensuse::OCIHelper("org.opensuse.OCIHelper", "/", QDBusConnection::systemBus(), this);
    if ( !m_ociHelper->isValid() ) {
	qFatal( "Oops! Cannot connect to the service org.opensuse.OCIHelper" );
	exit( 1 );
    }
    
    m_mediaCallbacks = new org::opensuse::MediaCallbacks("org.opensuse.MediaCallbacks", "/Media", QDBusConnection::systemBus(), this);
    if ( !m_mediaCallbacks->isValid() ) {
	qFatal( "Oops! Cannot connect to the service org.opensuse.MediaCallbacks" );
	exit( 1 );
    }
    
    qDebug() << " DBus proxies initialized";
    
    // signals and slots
    connect( m_mediaCallbacks, SIGNAL( startProgress( QString ) ), this, SLOT( newProgressInAction( QString ) ) );
    connect( m_mediaCallbacks, SIGNAL( startResolvable( QString ) ), this, SLOT( newResolvableInAction( QString ) ) );    
    connect( m_mediaCallbacks, SIGNAL( finishProgress( QString, bool ) ), this, SLOT( updateCurrentProgressStatusUponCompletion( QString, bool ) ) );
    connect( m_mediaCallbacks, SIGNAL( finishResolvable( QString, bool ) ), this, SLOT( updateCurrentResolvableStatusUponCompletion( QString, bool ) ) );
    connect( m_mediaCallbacks, SIGNAL( progress( int ) ), m_progressBar, SLOT( setValue( int ) ) ); // update progress
}

// Invoke this every time startResolvable() is emitted from start() method[ in media.h ] except for 
// DownloadProgressReportReceiver in OCIHelper
void InstallScreen::newResolvableInAction( const QString & label_R )
{
    m_statusWidget->append( label_R );
}

// Invoke this every time finishResolvable() is emitted from finish() method[ in media.h ] except 
// DownloadProgressReportReceiver in OCIHelper
void InstallScreen::updateCurrentResolvableStatusUponCompletion( const QString & finish_R, bool success )
{
    m_statusWidget->append( finish_R );
    
    // set (incr) the total progress bar value
    m_totalProgressBar->setValue( ++m_currentPackage );
}

void InstallScreen::newProgressInAction( const QString & label_R )
{
    m_currentPackageStatusLabel->setText( label_R );
    // write it to the log file
    ( *m_outData ) << label_R << "\n";
    m_progressBar->reset();
}

void InstallScreen::updateCurrentProgressStatusUponCompletion( const QString & finish_R, bool success )
{
    // set the progress bar to 100
    m_progressBar->setValue( 100 );
    // write it to the log file
    ( *m_outData ) << finish_R << "\n";
    m_currentPackageStatusLabel->setText( finish_R );
}

QWidget* InstallScreen::horizontalLine()
{
    QWidget* horizontalLine = new QWidget;
    horizontalLine->setFixedHeight( 2 );
    horizontalLine->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    horizontalLine->setStyleSheet( QString( "background-color: #c0c0c0;" ) );
    return horizontalLine;
}

void InstallScreen::cancelInstallation()
{
    qDebug() << "cancelling installation";  
    
    closeLogFile();
    m_ociHelper->killBackend(); // quit OCIHelper
    qApp->quit(); // quit OCI
}

void InstallScreen::setTotalProgressMax( int commitPackages )
{
    /*
     * Set the max value for total progress bar - i.e. twice the number of packages.
     * When 50% is reached, it would mean "all packages are downloaded"
     */
    m_totalProgressBar->setMaximum( commitPackages * 2 );
}

void InstallScreen::closeLogFile()
{
    qDebug() << "closing file [ in installscreen ]";
    m_logFile.close();
}
