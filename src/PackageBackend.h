#ifndef PACKAGEBACKEND_H
#define PACKAGEBACKEND_H

#include <QList>

class PackageBackend
{
    public:
    virtual void addRepository( const QString& url  ) = 0;
    virtual bool performInstallation( const QList< QString >& pack) = 0;
    virtual bool resolveConflicts() = 0;
};

#endif
