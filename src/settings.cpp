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
#include "settings.h"

Settings::Settings( QSettings* settings, QObject *parent )
{
    setStyleSheet( "background-color : rgb(251,248,241)" );

    m_settings = settings;
    m_settings->sync();

    //Create Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *closeLayout = new QHBoxLayout;

    //Create Interface Elements
    m_repos = new QLabel( i18n("<b>Repository Sources</b>") );
    m_showDetails = new QCheckBox( i18n("Show repository details by default"), this );
    m_proposal = new QCheckBox( i18n("Always display an installation proposal"), this );
    m_close = new QPushButton( i18n("Close") );

    if( m_settings->childKeys().count() != 0 ) {
        if( m_settings->value( "proposal", 1 ).toInt() == 1 ) {
            m_proposal->setChecked( true );
        } else {
            m_proposal->setChecked( false );
        }

        if ( m_settings->value( "showdetails", 1 ).toInt() ) {
            m_showDetails->setChecked( true );
        } else {
            m_showDetails->setChecked( false );
        }
    }

    //Add elements to Layout
    mainLayout->addWidget( m_repos );
    mainLayout->addWidget( m_showDetails );
    mainLayout->addWidget( m_proposal );
    closeLayout->addSpacing(150);
    closeLayout->addWidget( m_close );
    mainLayout->addLayout( closeLayout );
    setLayout( mainLayout );
    setWindowTitle( i18nc("Please don't translate the application name. Use any of the following templates- [________ One Click Installer]; [One Click Installer ____________]", "One Click Installer Settings") );

    QObject::connect( m_close, SIGNAL(clicked()), this, SLOT( saveSettingsAndClose()) );
    show();
}

void Settings::saveSettingsAndClose()
{
    if( m_proposal->isChecked() ) {
        m_settings->setValue( "proposal", 1 );
    } else {
        m_settings->setValue( "proposal", 0 );
    }

    if( m_showDetails->isChecked() ) {
        m_settings->setValue( "showdetails", 1 );
    } else {
        m_settings->setValue( "showdetails", 0 );
    }

    m_settings->sync();

    close();
}
