#ifndef PACKAGEBACKEND_H
#define PACKAGEBACKEND_H

#include <QList>

class PackageBackend
{
    public:
    virtual void addRepository( const QString& url  ) = 0;
    virtual bool performInstallation() = 0;
    virtual bool resolveConflicts() = 0;
};

#endif
