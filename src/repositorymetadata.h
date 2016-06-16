#ifndef REPOSITORYMETADATA_H
#define REPOSITORYMETADATA_H

#include <iostream>
#include <string>
#include <QString>
#include "repository.h"
#include "repositorydata.h"

class RepositoryMetadata
{
public:

    /**
     * Construct the Object with Repository Object
     */
    RepositoryMetadata( OCI::Repository *repository );

    /**
     * Return the fingerprint
     */
    QString fingerprint();

    /**
     * Return the creation data
     */
    QString created();

    /**
     * Return the expiry date
     */
    QString expires();

    /**
     * Return the id
     */
    QString id();
private:
    RepositoryData *m_repoData;
};

#endif
