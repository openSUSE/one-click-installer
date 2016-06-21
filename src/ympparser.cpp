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


#include "ympparser.h"
#include <QDebug>
#include <QFile>

OCI::YmpParser::YmpParser( const QString& ympfile )
{
    m_fileName = ympfile;
}

QList< OCI::Package* > OCI::YmpParser::packages() const
{
    return m_packageList;
}

QList< OCI::Repository* > OCI::YmpParser::repositories() const
{
    return m_repositoryList;
}

void OCI::YmpParser::parse()
{
    QFile file( m_fileName );
    if( !file.open( QIODevice::ReadOnly ) ) {
        qDebug()<<"Could not open File";
        return;
    }
    QString fileData( file.readAll() );
    //qDebug()<<fileData;
    QXmlStreamReader xml( fileData );

    while( !xml.atEnd() && xml.name() != "software" ) {
        xml.readNextStartElement();
        if( xml.name()=="repository" && !xml.isEndElement() ) {
            OCI::Repository *repo = new OCI::Repository;

            //Set whether recommended or not
            repo->setRecommended( xml.attributes().value( "recommended" ).toString() );
            xml.readNextStartElement();

            //Read the Name of the Repository
            if( xml.name() == "name" ) {
                //qDebug()<<"Name"<<xml.readElementText();
                repo->setName( xml.readElementText() );
            }
            xml.readNextStartElement();

            //Read the Summary
            if( xml.name()=="summary" ) {
                //qDebug()<<"Summary"<<xml.readElementText();
                repo->setSummary( xml.readElementText() );
            }
            xml.readNextStartElement();

            //Read Description
            if( xml.name() == "description" ) {
                //qDebug()<<"Description"<<xml.readElementText();
                repo->setDescription( xml.readElementText() );
            }
            xml.readNextStartElement();

            //Read Url
            if( xml.name() == "url" ) {
                repo->setUrl( xml.readElementText() );
            }

            //Add Repository to the List or Repositories
            m_repositoryList.append( repo );
        }
    }

    while( !xml.atEnd() && !( xml.name() == "software" && xml.isEndElement() ) ) {
        xml.readNextStartElement();
        if( xml.name() == "name" && !xml.isEndElement() ) {
            OCI::Package *pkg = new Package;

            //Read Element Text
            pkg->setName( xml.readElementText() );
            xml.readNextStartElement();

            //Read Summary
            if( xml.name() == "summary" )
                pkg->setSummary( xml.readElementText() );
            xml.readNextStartElement();

            //Read Description
            if( xml.name() == "description" )
                pkg->setDescription( xml.readElementText() );
            m_packageList.append( pkg );
        }
    }
}

void OCI::YmpParser::printRepoList()
{
    foreach( OCI::Repository* repo, m_repositoryList ) {
        qDebug() << repo->name();
        qDebug() << repo->description();
        qDebug() << repo->url();
        qDebug() << repo->summary();
        qDebug() << repo->recommended();
    }
}

void OCI::YmpParser::printPackageList()
{
    foreach( OCI::Package* pack, m_packageList ) {
        qDebug() << pack->name();
        qDebug() << pack->description();
        qDebug() << pack->summary();
    }
}
