#ifndef YMPPARSER_H
#define YMPPARSER_H

#include <QXmlStreamReader>
#include <QList>

#include "package.h"
#include "repository.h"

class YmpParser
{
	public:
		YmpParser(const QString& ympfile);
		void parse();
		QList<Package*> packages() const;
		QList<Repository*> repositories() const;
		void printRepoList();
		void printPackageList();
	private:
		QList<Package*> packageList;
		QList<Repository*> repositoryList;
		QString fileName;
};

#endif
