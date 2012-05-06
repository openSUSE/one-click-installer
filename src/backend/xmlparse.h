#ifndef XMLPARSE_H
#define XMLPARSE_H

#include <QXmlStreamReader>
#include <QList>

#include "package.h"
#include "repository.h"

class XMLParse
{
	public:
		XMLParse(const QString& ympfile);
		void parse();
		QList<Package*> packages();
		QList<Repository*> repositories();
		void printRepoList();
		void printPackageList();
	private:
		QList<Package*> packageList;
		QList<Repository*> repositoryList;
		QString fileName;
};

#endif
