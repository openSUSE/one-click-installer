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

int main(int argc,char *argv[])
{
	QApplication app(argc,argv);
	QFile file(argv[1]);
	QList<Package*> packageList;
	QList<Repository*> repositoryList;
	zypp::RepoManager rman;
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug()<<"Could not open File";
		return 0;
	}
	QString fileData(file.readAll());
	//qDebug()<<fileData;
	QXmlStreamReader xml(fileData);

	while(!xml.atEnd() && xml.name()!="software")
	{
		xml.readNextStartElement();
		if(xml.name()=="repository" && !xml.isEndElement())
		{
			Repository *repo = new Repository;
			//Set whether recommended or not
			repo->setRecommended(xml.attributes().value("recommended").toString());

			xml.readNextStartElement();
			//Read the Name of the Repository 
			if(xml.name()=="name")
			{
				//qDebug()<<"Name"<<xml.readElementText();
				repo->setName(xml.readElementText());
			}
			
			xml.readNextStartElement();
			//Read the Summary
			if(xml.name()=="summary")
			{
				//qDebug()<<"Summary"<<xml.readElementText();
				repo->setSummary(xml.readElementText());
			}
			xml.readNextStartElement();
			//Read Description
			if(xml.name()=="description")
			{
				//qDebug()<<"Description"<<xml.readElementText();
				repo->setDescription(xml.readElementText());
			}
			xml.readNextStartElement();
			//Read Url
			if(xml.name()=="url")
			{
				repo->setUrl(xml.readElementText());
			}
			
			//Add Repository to the List or Repositories
			repositoryList.append(repo);

		}	
	}


	while(!xml.atEnd() && !(xml.name()=="software" && xml.isEndElement()))
	{
		xml.readNextStartElement();
		if(xml.name()=="name" && !xml.isEndElement())
		{
			Package *pkg = new Package;
			//Read Element Text
			pkg->setName(xml.readElementText());

			xml.readNextStartElement();

			//Read Summary
			if(xml.name()=="summary")
				pkg->setSummary(xml.readElementText());
			xml.readNextStartElement();

			//Read Description
			if(xml.name()=="description")
				pkg->setDescription(xml.readElementText());
			packageList.append(pkg);
		}
	}
	
	qDebug()<<"***List of Repositories***";
	foreach(Repository *repo,repositoryList)
	{
		qDebug() << repo->name();
		qDebug() << repo->recommended();
		qDebug() << repo->summary();
		qDebug() << repo->description();
		qDebug() << repo->url();
		//Add Repository
		zypp::RepoInfo repoinfo;
		std::cout<<"Std Url is "<<repo->url().toStdString()<<std::endl;
		repoinfo.addBaseUrl(zypp::Url(repo->url().toStdString()));
		repoinfo.setAlias(repo->url().toStdString());
		repoinfo.setGpgCheck(false);
		rman.addRepository(repoinfo);
		rman.refreshMetadata(repoinfo,zypp::RepoManager::RefreshIfNeeded);
		rman.buildCache(repoinfo);
		rman.loadFromCache(repoinfo);

	}
	qDebug() << "***List of Packages***" ;
	foreach(Package *pack,packageList)
	{
		qDebug() << pack->name();
		qDebug() << pack->summary();
		qDebug() << pack->description();
	}

	return 0;
}
