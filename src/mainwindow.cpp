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

#include <QDBusConnection>
#include <klocalizedstring.h>
#include <sys/types.h>
#include <unistd.h>
#include "mainwindow.h"
#include "checkconflictscreen.h"

MainWindow::MainWindow( const QString& filename, const QString& tmpFileName, bool fakeRequested, QObject *parent )
{
    QDBusConnection sysBus = QDBusConnection::systemBus();
    
    setStyleSheet( "background-color : rgb(251,248,241)" );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setMinimumSize( 600, 400 );

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QHBoxLayout *m_warningLayout = new QHBoxLayout;

    m_showSettings = new QPushButton( i18n("Settings") );
    m_cancel = new QPushButton( i18n("Cancel") );
    m_install = new QPushButton( i18n("Install") );

    buttonLayout->addWidget( m_showSettings );
    buttonLayout->addSpacing( 100 );
    buttonLayout->addWidget( m_cancel );
    buttonLayout->addSpacing( 10 );
    buttonLayout->addWidget( m_install );

    QObject::connect( m_showSettings, SIGNAL( clicked() ), this, SLOT( showSettings() ) );
    QObject::connect( m_install, SIGNAL( clicked() ), this, SLOT( performInstallation() ) );
    QObject::connect( m_cancel, SIGNAL( clicked()), this, SLOT( close() ) );

    m_tmpFileName = tmpFileName;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_screenStack = new QStackedLayout;

    //Set up the backend
    if( fakeRequested ) {
        m_backend = new FakeBackend( this );
        m_fakeRequested = true;
    } else {
        m_backend = new BackendOCI( m_tmpFileName, (int)getuid() );
        m_fakeRequested = false;
    }

    m_header = new MainHeader;

    m_firstScreen = new FirstScreen( m_backend, m_tmpFileName, filename, this );
    Summary *installSummary = new Summary( m_backend, m_tmpFileName );
    InstallScreen *installer = new InstallScreen( m_backend, m_tmpFileName );
    CheckConflictScreen *conflictAnimation = new CheckConflictScreen();
    
    QScrollArea *scroll = new QScrollArea;
    scroll->setFrameShape( QFrame::NoFrame );
    scroll->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scroll->setWidget( m_firstScreen );
    scroll->setWidgetResizable( true );
    
    
    m_screenStack->addWidget( scroll );
    m_screenStack->addWidget( installSummary );
    m_screenStack->addWidget( installer );
    m_screenStack->addWidget( conflictAnimation );

    m_screenStack->setCurrentIndex( 0 );

    mainLayout->addWidget( m_header );
    mainLayout->addLayout( m_screenStack );
    mainLayout->addSpacing( 5 );
    mainLayout->addLayout( m_warningLayout );
    mainLayout->addSpacing( 20 );
    mainLayout->addLayout( buttonLayout );

    setLayout( mainLayout );
    setWindowTitle("One Click Installer");
    setWindowIcon( QIcon("/usr/share/icons/hicolor/32x32/apps/oneclickinstall.png") );

    QObject::connect( m_firstScreen, SIGNAL( showNextScreen( int ) ), this, SLOT( showNextScreen( int ) ) );
    QObject::connect( installSummary, SIGNAL( showNextScreen( int ) ), this, SLOT( showNextScreen( int ) ) );
    QObject::connect( m_firstScreen, SIGNAL( countChanged( int, int )), this, SLOT( updateCount( int, int ) ) );
    QObject::connect( this, SIGNAL( countChanged( int, int ) ), m_header, SLOT( changeStatusLabel( int, int) ) );
    QObject::connect( m_firstScreen, SIGNAL( sizeUpdated( QString ) ), this, SLOT( updateSize( QString ) ) );
    QObject::connect( m_firstScreen, SIGNAL( packageListInstallableStateToggled( bool ) ), m_install, SLOT( setEnabled( bool ) ) );

    // For Conflict Resolution
    QObject::connect( m_backend, SIGNAL( checkForConflicts() ), this, SLOT( showCheckForConflictsProgress() ) );
    QObject::connect( m_backend, SIGNAL( checkForConflicts() ), m_header, SLOT( showCheckForConflictsHeader() ) );
    sysBus.connect( QString(), QString(), "org.opensuse.oneclickinstaller", "hasConflicts", m_header, SLOT( showConflictResolutionHeader() ) );
     /* For Installation 
     * QObject::connect( , SIGNAL( installationStarted() ), , SLOT( installationStarted() ) );
     * QObject::connect( , SIGNAL( installationCompleted() ), , SLOT( installationCompleted() ) );
     * QObject::connect( , SIGNAL( installationCompleted() ), m_header, SLOT( showCompletionStatus() ) );
     * QObject::connect( , SIGNAL( installationCompleted() ), m_header, SLOT( installationCompleted() ) );
     */
    show();
}

void MainWindow::showNextScreen( int index )
{
    m_screenStack->setCurrentIndex( index );
}

void MainWindow::showCheckForConflictsProgress()
{
    m_screenStack->setCurrentIndex( 3 );
    
    m_install->hide();
    m_showSettings->hide();
    m_cancel->hide();
}

void MainWindow::updateCount( int repoCount, int packageCount )
{
    emit countChanged( repoCount, packageCount );
}


void MainWindow::showSettings()
{
    new Settings( &m_settings );
}

void MainWindow::performInstallation()
{
    if( m_settings.value( "proposal" ).toInt() == 1 ) {
        m_screenStack->setCurrentIndex( 1 );
    } else {
        //m_screenStack->setCurrentIndex( 2 );
        m_backend->callBackendHelper();
    }
    m_install->hide();
    m_showSettings->hide();
    m_cancel->hide();
}

void MainWindow::updateSize( const QString& size )
{
    if( m_screenStack->currentIndex() == 0 )
        m_header->updateDetails( size );
}
