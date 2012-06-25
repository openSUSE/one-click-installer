#include "PackageBackend.h"

void PackageBackend::addRepository( const QUrl& url )
{
    m_repositories.append( url );
}

void PackageBackend::addPackage( const QString& package )
{
    m_packages << package;
}

bool PackageBackend::resolveConflicts()
{
}

QList< QString > PackageBackend::packages()
{
    return m_packages;
}

QList< QUrl > PackageBackend::repositories()
{
    return m_repositories;
}
