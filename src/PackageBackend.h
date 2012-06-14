#ifndef PACKAGEBACKEND_H
#define PACKAGEBACKEND_H

#include <QList>
#include <QUrl>

class PackageBackend
{
    public:
    virtual void setToAddRepository( const QUrl& url  ) = 0;
    virtual void performInstallation( const QString& pack) = 0;
    virtual bool resolveConflicts() = 0;
};

#endif
