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


#include <unistd.h>
#include <klocalizedstring.h>
#include "summary.h"

Summary::Summary(PackageBackend *backend, const QString& tmpFileName, QObject *parent )
{
    m_backend = backend;
    m_tmpFileName = tmpFileName;
    m_installationSummary = new QTextBrowser;
    m_installationSummary->addScrollBarWidget( new QScrollBar, Qt::AlignRight );
    m_continue = new QPushButton( i18n("Continue Installation") );
    m_cancel = new QPushButton( i18n("Cancel") );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    mainLayout->addWidget( m_installationSummary );
    buttonsLayout->addSpacing( 100 );
    buttonsLayout->addWidget( m_cancel );
    buttonsLayout->addSpacing( 20 );
    buttonsLayout->addWidget( m_continue );
    mainLayout->addLayout( buttonsLayout );
    setLayout( mainLayout );

    foreach( QUrl repos, m_backend->repositories() ) {
        m_installationSummary->append( i18n("Add Source <b>%1</b>").arg(repos.toString()) );
    }

    foreach( const QString package, m_backend->packages() ) {
        m_installationSummary->append( i18n("Install <b>%1</b>").arg(package) );
    }

    QObject::connect( m_continue, SIGNAL( clicked() ), this, SLOT( continueInstallation() ) );
    QObject::connect( m_cancel, SIGNAL( clicked() ), this, SLOT( cancel() ) );
}

void Summary::continueInstallation()
{
    emit showNextScreen( 2 );
    m_backend->callBackendHelper();
}

void Summary::cancel()
{
    exit( 0 );
}
