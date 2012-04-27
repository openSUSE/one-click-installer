#include <QApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QStringList>
#include "repository.h"
#include "package.h"

int main(int argc,char *argv[])
{
	QApplication app(argc,argv);
	QFile file(argv[1]);
	QList<Package*> packageList;
	QList<Repository*> repositoryList;
	
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
		xml.readNext();
		if(xml.name()=="repository" && !xml.isEndElement())
		{
			//Add a New Repository to the List
			Repository *repo = new Repository;
			xml.readNext();
			xml.readNext();
			//qDebug()<<xml.name();
			if(xml.name()=="name")
				repo->setName(xml.readElementText());
			repositoryList.append(repo);

		}	
	}

	//qDebug()<<xml.name();

	while(!xml.atEnd() && !(xml.name()=="software" && xml.isEndElement()))
	{
		xml.readNext();
		if(xml.name()=="name" && !xml.isEndElement())
		{
			Package *pkg = new Package;
			pkg->setName(xml.readElementText());
			packageList.append(pkg);
		}
	}
	
	qDebug()<<"***List of Repositories***";
	foreach(Repository *repo,repositoryList)
	{
		qDebug()<<repo->name();
	}
	qDebug()<<"***List of Packages***";
	foreach(Package *pack,packageList)
	{
		qDebug()<<pack->name();
	}

	return 0;
}
