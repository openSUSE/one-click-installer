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


#include "repositorywidget.h"

RepositoryWidget::RepositoryWidget(PackageBackend *backend, int index, OCI::Repository *repo, QObject *parent )
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

    m_settings.sync();

    m_backend = backend;

    m_repo = repo;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *sourceLayout = new QHBoxLayout;
    mainLayout->setSpacing( 0 );

    m_name = new QLabel( QString( "Source: <b>%1</b>" ).arg( repo->name() ) );
    m_showDetails = new QLabel( QString( "<a href = %1>Show Details</a>" ).arg( index ) );

    m_name->setContentsMargins( 10, 10, 10, 10 );
    m_showDetails->setContentsMargins( 10, 10, 10, 10 );

    m_name->setStyleSheet( "background-color: rgb(254, 250, 210); border-bottom : 1px solid rgb(252,233,79); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147);" );
    m_showDetails->setStyleSheet( "background-color: rgb(254, 250, 210); border-bottom : 1px solid rgb(252,233,79); border-right : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147);" );

    sourceLayout->addWidget( m_name );
    sourceLayout->addWidget( m_showDetails );

    mainLayout->addLayout( sourceLayout );

    m_detailsVisible = false;

    QObject::connect( m_showDetails, SIGNAL( linkActivated( QString ) ), this, SLOT( showDetails(QString ) ) );

    m_url = new QLabel( QString( "<b>URL :</b> %1" ).arg( repo->url() ) );

    m_url->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px; border-right : 1px solid rgb(196,181,147); border-left : 1px solid rgb(196,181,147);" );

    setLayout( mainLayout );

    if( m_settings.value( "showdetails", 1 ).toInt() == 1 ) {
        showDetails( QString( "%1" ).arg( index ) );
    }

    m_visible = false;
}

void RepositoryWidget::showDetails( QString link )
{
    if( !m_visible ) {
        m_meta = new RepositoryMetadata( m_repo );
        m_meta->refresh();
        m_id = new QLabel( QString( "<b>ID:</b> %1" ).arg( m_meta->id() ) );
        m_fingerprint = new QLabel( QString( "<b>Fingerprint:</b> %1" ).arg( m_meta->fingerprint() ) );
        m_created = new QLabel( QString( "<b>Created:</b> %1" ).arg( m_meta->created() ) );
        m_expires = new QLabel( QString( "<b>Expires :</b> %1" ).arg( m_meta->expires() ) );
        m_id->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px; border-right : 1px solid rgb(196,181,147); border-left : 1px solid rgb(196,181,147);" );
        m_fingerprint->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px; border-right : 1px solid rgb(196,181,147); border-left : 1px solid rgb(196,181,147);" );
        m_created->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px; border-right : 1px solid rgb(196,181,147); border-left : 1px solid rgb(196,181,147);" );
        m_expires->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px; border-right : 1px solid rgb(196,181,147); border-left : 1px solid rgb(196,181,147);" );
        m_visible = true;
        delete m_meta;
    }

    int linkNo = link.toInt();
    if( m_detailsVisible ) {
        m_url->hide();
        m_fingerprint->hide();
        m_created->hide();
        m_expires->hide();
        m_id->hide();

        m_detailsVisible = false;

        m_showDetails->setText( QString( "<a href = %1>Show Details</a>" ).arg( linkNo ) );
    } else {
        this->layout()->addWidget( m_url );
        this->layout()->addWidget( m_id );
        this->layout()->addWidget( m_fingerprint );
        this->layout()->addWidget( m_created );
        this->layout()->addWidget( m_expires );

        m_url->show();
        m_fingerprint->show();
        m_created->show();
        m_expires->show();

        m_detailsVisible = true;

        m_showDetails->setText( QString( "<a href = %1>Hide Details</a>" ).arg( linkNo ) );
    }
}
