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
    RepositoryMetadata( OCI::Repository *repository );
    QString fingerprint();
    QString created();
    QString expires();
    QString id();

    void refresh();
private:
    RepositoryData *m_repoData;
};

#endif
