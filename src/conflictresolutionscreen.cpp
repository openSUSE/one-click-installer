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

#include <QDBusConnection>
#include <QRadioButton>
#include "conflictresolutionscreen.h"

ConflictResolutionScreen::ConflictResolutionScreen()
{
    setStyleSheet( "background-color: white;" );
    
    QDBusConnection sysBus = QDBusConnection::systemBus();
    m_ociHelper = new org::opensuse::OCIHelper("org.opensuse.OCIHelper", "/", sysBus, this);
    m_mainLayout = new QVBoxLayout;
    m_cancelInstallation = new QPushButton( "Cancel" );
    m_continueInstallation = new QPushButton( "Continue Installation" );
    
    // Emit signal displayProblemSolution( QString, QString ) - First string = problem, second string = solutions
    // Each solution is separated by a "/"
    sysBus.connect( QString(), QString(), "org.opensuse.OCIHelper", "displayProblemAndSolutions", this, SLOT( problemSolutionWidget( QString, QString ) ) );
    
    setLayout ( m_mainLayout );
}

void ConflictResolutionScreen::problemSolutionWidget( QString probDescription, QString solutions )
{
    QLabel *questionLabel = new QLabel( probDescription );
    questionLabel->setStyleSheet( "background-color: rgb(254, 250, 210); border-bottom : 1px solid rgb(252,233,79); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147);" );
    questionLabel->setFixedHeight( 40 );
    
    QWidget *solutionWidget = new QWidget;
    solutionWidget->setObjectName( "solutionWidget" );
    solutionWidget->setStyleSheet( "QWidget#solutionWidget{ background-color : white; border-bottom: 1px solid rgb(196, 181, 147); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); }" );
    
    QVBoxLayout *solutionLayout = new QVBoxLayout( solutionWidget );
    solutionLayout->setSpacing( 0 );
    
    for ( QString solution : solutions.split( "/", QString::SkipEmptyParts) ) {
	QRadioButton *sol = new QRadioButton( solution );
	sol->setFixedHeight( 25 );
	sol->setStyleSheet( "padding-left: 40px;" );
	solutionLayout->addWidget( sol );
	solutionLayout->setSpacing( 10 );
    }
    
    m_mainLayout->addWidget( questionLabel );
    m_mainLayout->setSpacing( 0 );
    m_mainLayout->addWidget( solutionWidget );
}
void ConflictResolutionScreen::cancelInstallation() 
{
    qDebug() << "cancelling installation";
    m_ociHelper->killBackend();
    qApp->quit();
}

void ConflictResolutionScreen::sendSolutionToOCIHelper()
{
    /*
     * Get the main layout  QLayout *layout = QWidget::layout();
     * Access its children (especially solutionWidget)
     * Check if QRadioButton is checked - by default it'll be the first one 
     * Figure something out to uniquely identify the solution (like id number may be)
     * send it to OCIHelper
     * 
     * This is similar to what we've done to disable Install Button when no packages
     * are selected.
     */
}
