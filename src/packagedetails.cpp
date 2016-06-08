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
#include "packagedetails.h"

PackageDetails::PackageDetails(OCI::Package *package,int count, int packagecount, QObject *parent )
{
    setObjectName( "pk" );
    setStyleSheet( "#pk{background-color : white; border : 1px solid rgb(196,181,147);}");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing( 0 );
    QHBoxLayout *packageLayout = new QHBoxLayout;

    meta = new PackageMetadata( package->name() );
    meta->getData();

    m_summary = new QLabel( i18n("<b>Summary:</b> %1").arg(package->summary()) );
    m_fetchingAnimation.setFileName("/usr/share/one-click-installer/res/ticks-endless.gif");
    m_version = new QLabel( i18n("Fetching...") );
    m_size = new QLabel( i18n("Fetching...") );
    if (m_fetchingAnimation.isValid()) {
       m_version->setMovie(&m_fetchingAnimation);
       m_size->setMovie(&m_fetchingAnimation);
       m_fetchingAnimation.start();
    };

    QObject::connect( meta, SIGNAL( finished( QString,QString ) ), this, SLOT( dataChanged( QString,QString ) ) );

     m_showDescription = new QLabel( QString( "<a href = \"%1\">%2</a>" ).arg( count ).arg( i18n("Show Details") ));
    m_showDescription->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    m_description = new QLabel( QString( "%1" ).arg( package->description() ) );
    m_description->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );

    m_descriptionVisible = false;

    QObject::connect( m_showDescription, SIGNAL( linkActivated( QString ) ), this, SLOT( showPackageDescription( QString ) ) );

    m_size->setStyleSheet( "background-color : white;  padding-left : 3px;" );
    m_size->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    m_version->setStyleSheet( "background-color : white; padding-left : 3px;" );
    m_version->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    m_summary->setStyleSheet( "background-color : white; border-left : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); padding-left : 3px; border-bottom : 1px solid rgb(196,181,147);" );
    m_summary->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    m_showDescription->setStyleSheet( "background-color : white;border-right : 1px solid rgb(196,181,147); " );
    m_showDescription->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    m_description->setStyleSheet( "background-color : white; border-bottom : 1px solid rgb(196,181,147); border-left : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147);" );
    m_description->setContentsMargins( 10, 10, 10, 10 );
    m_description->setWordWrap( true );

    if( packagecount == 1 ) {
        m_singlePackage = new QLabel( package->name() );
        m_singlePackage->setStyleSheet( "background-color : white; border-left : 1px solid rgb(196,181,147);" );
        m_singlePackage->setContentsMargins( 10, 10, 10, 10 );
        m_singlePackage->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
        packageLayout->addWidget( m_singlePackage );
    } else {
        m_packageName = new QCheckBox( package->name() );
        m_packageName->setChecked( true );
        m_packageName->setContentsMargins( 10, 10, 10, 10 );
        m_packageName->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Expanding );
        m_packageName->setStyleSheet( "background-color : white; border-left : 1px solid rgb(196,181,147); padding-left : 3px;" );
        QObject::connect( m_packageName, SIGNAL( toggled( bool ) ), this, SIGNAL( installableStateToggled( bool )  ) );
        packageLayout->addWidget( m_packageName );
    }
    
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
        m_showDescription->setText( QString( "<a href = \"%1\">%2</a>" ).arg( linkNo ).arg( i18n("Show Details") ) );
        m_description->hide();
        m_descriptionVisible = false;
        m_summary->setStyleSheet( "background-color : white; border-left : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); padding-left : 3px; border-bottom : 1px solid rgb(196,181,147);" );
    } else {
        m_showDescription->setText( QString( "<a href = \"%1\">%2</a>" ).arg( linkNo ).arg( i18n("Hide Details") ) );
        this->layout()->addWidget( m_description );
        m_description->show();
        m_descriptionVisible = true;
        m_summary->setStyleSheet( "background-color : white; border-left : 1px solid rgb(196,181,147); border-right : 1px solid rgb(196,181,147); padding-left : 3px;" );
    }
}

void PackageDetails::dataChanged( QString version, QString size )
{
    m_version->setText( version );
    m_size->setText( size );

    emit sizeUpdated( size );
}

bool PackageDetails::shouldBeInstalled() const
{
    if (!m_packageName)
        return false;
    return m_packageName->isChecked();
}
