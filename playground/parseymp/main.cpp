#include <QApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QStringList>

int main(int argc,char *argv[])
{
	QApplication app(argc,argv);
	QFile file(argv[1]);
	
	QStringList packageList;
	QStringList repositoryList;
	
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
			//qDebug()<<"in repo\n";
			xml.readNext();
			xml.readNext();
			qDebug()<<xml.name();
			if(xml.name()=="name")
				repositoryList<<xml.readElementText();
		}	
	}

	//qDebug()<<xml.name();

	while(!xml.atEnd() && !(xml.name()=="software" && xml.isEndElement()))
	{
		xml.readNext();
		if(xml.name()=="name" && !xml.isEndElement())
		{
			packageList<<xml.readElementText();
		}
	}
	
	qDebug()<<"***List of Repositories***";
	foreach(QString repo,repositoryList)
	{
		qDebug()<<repo;
	}
	qDebug()<<"***List of Packages***";
	foreach(QString pack,packageList)
	{
		qDebug()<<pack;
	}

	return 0;
}
