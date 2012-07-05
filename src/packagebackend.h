#ifndef PACKAGEBACKEND_H
#define PACKAGEBACKEND_H

#include <QList>
#include <QUrl>

class PackageBackend
{
    public:
    /**
     	Adds the repository required to install the selected packages. This will be added to the system as installation and update repository
     */
    virtual void addRepository( const QUrl& url  );

    /**
    	Add the package to be installed
     */
    virtual void addPackage( const QString& package);

    /**
    	Resolve Conflicts that may arise out of selecting packages
     */
    virtual bool resolveConflicts();

    /**
     	Return the list of packages selected for installation
     */
    virtual QList< QString > packages();

    /**
    	Return the list of repositories to be added
     */
    virtual QList< QUrl > repositories(); 

    /**
    	Carry out the installation and adding of repositories
     */
    virtual void install() = 0;

private:
    QList< QString > m_packages;
    QList< QUrl > m_repositories;

};

#endif
