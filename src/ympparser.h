#ifndef YMPPARSER_H
#define YMPPARSER_H

#include <QXmlStreamReader>
#include <QList>

#include "package.h"
#include "repository.h"

namespace OCI {
class YmpParser
{
public:
    /**
     * @brief YmpParser
     * @param ympfile : ymp filename
     */
    YmpParser( const QString& ympfile );

    /**
        Parse the YMP file to extract the contents
    */
    void parse();

    /**
        Returns the list of packages from the parsing
    */
    QList< OCI::Package* > packages() const;

    /**
        Returns the list of repositories from the parsing
    */
    QList< OCI::Repository* > repositories() const;

    void printRepoList();
    void printPackageList();
private:
    QList< OCI::Package* > packageList;
    QList< OCI::Repository* > repositoryList;
    QString fileName;
};
}
#endif
