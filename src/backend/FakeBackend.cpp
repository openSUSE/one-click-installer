#include "FakeBackend.h"

FakeBackend::FakeBackend()
{
    m_allTrusted = false;
}

void FakeBackend::parseYmpFile()
{
    packageList << QString( "Sample Package 1" );
    packageList << QString( "Sample Package 2" );
    repoList << QString( "Sample Repository 1" );
    repoList << QString( "Sample Repository 2" );
}

void FakeBackend::addRepository()
{

}

bool FakeBackend::performInstallation()
{

}

bool FakeBackend::resolveConflicts()
{

}

QList< QString > FakeBackend::repos()
{
    return repoList;
}

QList< QString >FakeBackend::packages()
{
    return packageList;
}
