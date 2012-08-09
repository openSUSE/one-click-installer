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


#include "packagedetails.h"

PackageDetails::PackageDetails(OCI::Package *package, int count, QObject *parent )
{
    setStyleSheet( "background-color : white" );
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing( 0 );
    QHBoxLayout *packageLayout = new QHBoxLayout;

    meta = new PackageMetadata( package->name() );
    meta->getData();

    m_summary = new QLabel( QString( "<b>Summary:</b> %1" ).arg( package->summary() ) );
    m_version = new QLabel( "Fetching..." );
    m_size = new QLabel( "Fetching..." );

    QObject::connect( meta, SIGNAL( finished( QString,QString ) ), this, SLOT( dataChanged( QString,QString ) ) );

    m_packageName = new QCheckBox( package->name() );
    m_packageName->setChecked( true );
    m_showDescription = new QLabel( QString( "<a href = %1>Show Details</a>" ).arg( count ) );
    m_description = new QLabel( QString( "%1" ).arg( package->description() ) );

    m_descriptionVisible = false;

    QObject::connect( m_showDescription, SIGNAL( linkActivated( QString ) ), this, SLOT( showPackageDescription( QString ) ) );

    m_packageName->setContentsMargins( 10, 10, 10, 10 );
    m_packageName->setStyleSheet( "background-color : white; border-left : 1px solid rgb(196,181,147);  padding-top : 20px; padding-bottom : 20px; padding-left : 3px;" );
    m_size->setStyleSheet( "background-color : white; padding-top : 20px; padding-bottom : 20px; padding-left : 3px;" );
    m_version->setStyleSheet( "background-color : white; padding-top : 20px; padding-bottom : 20px; padding-left : 3px;" );
    m_summary->setStyleSheet( "background-color : white; border-left : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); padding-top : 10px; padding-bottom : 10px; padding-left : 3px;" );

    m_showDescription->setStyleSheet( "background-color : white;border-right : 1px solid rgb(196,181,147); " );

    m_description->setStyleSheet( "background-color : white; border-bottom : 1px solid rgb(196,181,147); border-left : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147);" );
    m_description->setContentsMargins( 10, 10, 10, 10 );

    packageLayout->addWidget( m_packageName );
    packageLayout->addWidget( m_version );
    packageLayout->addWidget( m_size );
    packageLayout->addWidget( m_showDescription );
    mainLayout->addLayout( packageLayout );
    mainLayout->addWidget( m_summary );
    setLayout( mainLayout );
}

void PackageDetails::showPackageDescription( QString link )
{
    int linkNo = link.toInt();
    qDebug() << linkNo;

    if( m_descriptionVisible ) {
        m_showDescription->setText( QString( "<a href = %1>Show Details</a>" ).arg( linkNo ) );
        m_description->hide();
        m_descriptionVisible = false;
    } else {
        m_showDescription->setText( QString( "<a href = %1>Hide Details</a>" ).arg( linkNo ) );
        this->layout()->addWidget( m_description );
        m_description->show();
        m_descriptionVisible = true;
    }
}

void PackageDetails::dataChanged( QString version, QString size )
{
    m_version->setText( version );
    m_size->setText( size );

    emit sizeUpdated( size );
}
