#ifndef REPOSITORYMETADATA_H
#define REPOSITORYMETADATA_H

#include <iostream>
#include <string>
#include <QString>
#include "repository.h"

class RepositoryMetadata
{
public:
    RepositoryMetadata( OCI::Repository *repository );
    QString fingerprint();
    QString created();
    QString expires();
    QString id();
};

#endif
