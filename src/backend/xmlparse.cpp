#include "xmlparse.h"
#include <QDebug>
#include <QFile>

XMLParse::XMLParse(const QString& ympfile)
{
	fileName = ympfile;
}

QList<Package*> XMLParse::packages()
{
	return packageList;
}

QList<Repository*> XMLParse::repositories()
{
	return repositoryList;
}

void XMLParse::parse()
{
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug()<<"Could not open File";
		return;
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
}

void XMLParse::printRepoList()
{
	foreach(Repository* repo, repositoryList)
	{
		qDebug() << repo->name();
		qDebug() << repo->description();
		qDebug() << repo->url();
		qDebug() << repo->summary();
		qDebug() << repo->recommended();
	}
}

void XMLParse::printPackageList()
{
	foreach(Package* pack, packageList)
	{
		qDebug() << pack->name();
		qDebug() << pack->description();
		qDebug() << pack->summary();
	}
}
