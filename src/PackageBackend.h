#ifndef PACKAGEBACKEND_H
#define PACKAGEBACKEND_H

#include <QList>
#include <QUrl>

class PackageBackend
{
    public:
    virtual void addRepository( const QUrl& url  );
    virtual void addPackage( const QString& package);
    virtual bool resolveConflicts();
    virtual QList< QString > packages();
    virtual QList< QUrl > repositories(); 
    virtual void install() = 0;

    private:
    QList< QString > m_packages;
    QList< QUrl > m_repositories;

};

#endif
