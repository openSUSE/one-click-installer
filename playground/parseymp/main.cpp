#include <iostream>
#include <QApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QStringList>
#include "repository.h"
#include "package.h"
#include <QUrl>
#include <zypp/RepoManager.h>
#include <zypp/base/Algorithm.h>
#include <zypp/ResFilters.h>
#include <zypp/ResStatus.h>
#include <zypp/ResPool.h>
#include <zypp/target/rpm/RpmDb.h>
#include <zypp/target/TargetException.h>
#include <zypp/ZYppCommit.h>
#include <zypp/base/Regex.h>
#include <zypp/sat/WhatProvides.h>
#include <zypp/ZYppFactory.h>
#include <zypp/misc/DefaultLoadSystem.h>
#include "keyring.h"

namespace zypp
{
	typedef std::list< PoolItem > PoolItemList;
}

int main( int argc,char *argv[] )
{
	QApplication app( argc,argv );
        QFile file;
        QList<zypp::RepoInfo> repos;
        static zypp::KeyRingCallbacks keyring_callbacks;
        //zypp::KeyRing::setDefaultAccept( zypp::KeyRing::ACCEPT_UNKNOWNKEY );
        bool addrepo = false;
        if ( argv[1] == "" && argv[2] == "") {
            std::cout<<"--Usage--"<<std::endl<<"./readymp [--addrepo] <ymp filename>"<<std::endl;
            return 0;
        }
        if ( QString( argv[1] ) == QString( "--addrepo" ) ) {
            file.setFileName( argv[2] );
            std::cout<<argv[2];
            addrepo = true;
        } else {
            file.setFileName( argv[1] );
            std::cout<<argv[1];
        }
	QList< Package* > packageList;
	QList< Repository* > repositoryList;
	zypp::RepoManager rman;
	if( !file.open( QIODevice::ReadOnly ) ){
		qDebug() << "Could not open File";
		return 0;
	}
	QString fileData( file.readAll() );
	//qDebug()<<fileData;
	QXmlStreamReader xml( fileData );

	while( !xml.atEnd() && xml.name() != "software" ){
		xml.readNextStartElement();
		if( xml.name() == "repository" && !xml.isEndElement() ){
			Repository *repo = new Repository;
			//Set whether recommended or not
			repo->setRecommended( xml.attributes().value( "recommended" ).toString() );

			xml.readNextStartElement();
			//Read the Name of the Repository 
			if( xml.name() == "name" ){
				//qDebug()<<"Name"<<xml.readElementText();
				repo->setName( xml.readElementText() );
			}
			
			xml.readNextStartElement();
			//Read the Summary
			if( xml.name() == "summary" ){
				//qDebug()<<"Summary"<<xml.readElementText();
				repo->setSummary( xml.readElementText() );
			}
			xml.readNextStartElement();
			//Read Description
			if( xml.name() == "description" ){
				//qDebug()<<"Description"<<xml.readElementText();
				repo->setDescription( xml.readElementText() );
			}
			xml.readNextStartElement();
			//Read Url
			if(xml.name()=="url"){
				repo->setUrl( xml.readElementText() );
			}
			
			//Add Repository to the List or Repositories
			repositoryList.append( repo );

		}	
	}


	while( !xml.atEnd() && !( xml.name() == "software" && xml.isEndElement() ) ){
		xml.readNextStartElement();
		if( xml.name() =="name" && !xml.isEndElement() ){
			Package *pkg = new Package;
			//Read Element Text
			pkg->setName( xml.readElementText() );

			xml.readNextStartElement();

			//Read Summary
			if( xml.name() == "summary" )
				pkg->setSummary( xml.readElementText() );
			xml.readNextStartElement();

			//Read Description
			if( xml.name() == "description" )
				pkg->setDescription(xml.readElementText());
			packageList.append( pkg );
		}
	}
	
	qDebug() << "***List of Repositories***" ;
	foreach( Repository *repo,repositoryList ){
		qDebug() << repo->name();
		qDebug() << repo->recommended();
		qDebug() << repo->summary();
		qDebug() << repo->description();
		qDebug() << repo->url();
		//Add Repository
		if ( addrepo ){
                    zypp::RepoInfo repoinfo;
                    repos << repoinfo;
		    std::cout<<"Std Url is "<<repo->url().toStdString()<<std::endl;
		    repoinfo.addBaseUrl(zypp::Url(repo->url().toStdString()));
		    repoinfo.setAlias(repo->url().toStdString());
                    repoinfo.setGpgCheck(true);
		    rman.addRepository(repoinfo);
		    rman.refreshMetadata(repoinfo,zypp::RepoManager::RefreshIfNeeded);
		    rman.buildCache(repoinfo);
		    rman.loadFromCache(repoinfo);
                    std::cout<<std::endl<<"ALIAS IS "<<repoinfo.alias()<<std::endl;
                }

	}
	qDebug() << "***List of Packages***" ;
	foreach( Package *pack,packageList ){
		qDebug() << pack->name();
		qDebug() << pack->summary();
		qDebug() << pack->description();
	}
        bool ret = false;
        zypp::Resolvable::Kind kind = zypp::ResKind::package;
        //zypp::Arch architecture( "x86_64" );
        //std::string version_str = "1.0";
        //zypp::ZYpp::Ptr zypp_pointer = zypp::getZYpp();
        //try {
            //zypp::target::rpm::RpmDb rpmd;
            //zypp_pointer->initializeTarget( "/" );
        //}
        //catch( zypp::Exception &excpt ) {
        //    std::cout<<excpt.msg().c_str()<<std::endl;
        //}
        /*zypp::misc::defaultLoadSystem( "/" );
        zypp::ZYpp::Ptr zypp_pointer = zypp::getZYpp();
        zypp_pointer->initializeTarget( "/" );
        zypp::ResPoolProxy selectablePool( zypp::ResPool::instance().proxy() );
	zypp::ui::Selectable::Ptr s = zypp::ui::Selectable::get( packageList.at( 0 )->name().toStdString() );
        if ( !s ) {
            std::cout<<"Nothing is there\n";
            return 0;;
        }
        qDebug()<<"*****PRINTING POOL*********";
        foreach( const zypp::PoolItem &pi , zypp::ResPool::instance() ) {
            //std::cout<<pi<<std::endl;
        } 
        zypp::PoolItem p = s->highestAvailableVersionObj();
        std::cout<<"GRRRR "<<std::endl<<p<<std::endl;
        **for_( avail_it, s->availableBegin(), s->availableEnd()){
            zypp::Resolvable::constPtr res = p.resolvable();
	    s->setCandidate( p );
	    ret = s->setToInstall( zypp::ResStatus::USER );
	}
        s->setCandidate( p );
        ret = s->setToBeInstalled( zypp::ResStatus::USER );*/
        //zypp::ResPool pool = zypp::ResPool::instance();
        //p.status().setToBeInstalled( zypp::ResStatus::USER );
        
        //Resolving Dependencies

        /*bool rres= false;
        rres = zypp::ResPool::instance().resolver().resolvePool();
        if ( !rres ) {
            std::cout<<std::endl<<"Failed to Resolve Pool"<<std::endl;
        } else {
            std::cout<<"Resolved Pool"<<std::endl;
        }
        
        
        std::cout<<p.status()<<std::endl;
        **foreach (zypp::PoolItem pi,pool) {
            std::cout<<pi<<std::endl;
        }

        for (zypp::ResPool::byName_iterator it = pool.byNameBegin( "geany" ) ; it!=pool.byNameEnd( "geany" ) ; it++ ) {
            it->status().setToBeInstalled( zypp::ResStatus::USER );
        }*/

        /*zypp::ZYppCommitPolicy policy;
        policy.restrictToMedia( 0 );
        policy.downloadMode( zypp::DownloadInHeaps );
        policy.syncPoolAfterCommit( true );
        zypp::ZYppCommitResult result = zypp_pointer->commit( policy );
	if ( result.allDone() ) {
            std::cout<<"Installation done"<<std::endl;
        }*/
        return 0;
}
