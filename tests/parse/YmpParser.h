#ifndef YMPPARSER_H
#define YMPPARSER_H

#include <QXmlStreamReader>
#include <QList>

#include "package.h"
#include "repository.h"

namespace OCI{
class YmpParser
{
	public:
		YmpParser(const QString& ympfile);
		void parse();
		QList<OCI::Package*> packages() const;
		QList<OCI::Repository*> repositories() const;
		void printRepoList();
		void printPackageList();
	private:
		QList<OCI::Package*> packageList;
		QList<OCI::Repository*> repositoryList;
		QString fileName;
};
};
#endif
