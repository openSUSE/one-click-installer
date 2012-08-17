#ifndef PACKAGEBACKEND_H
#define PACKAGEBACKEND_H

#include <QList>
#include <QUrl>

class PackageBackend
{
    public:
    /**
     * @brief addRepository : add repository to repo list
     * @param url : url of repository
     */
    virtual void addRepository( const QUrl& url  );

    /**
     * @brief addPackage : adds package to package list
     * @param package : name of package
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

    /**
        Call the Backend Helper
     */
    virtual void callBackendHelper() = 0;

    /**
     * @brief setFileName : set temporary filename
     * @param fileName : temporary filename
     */
    virtual void setFileName( QString fileName );

    /**
        Get the Temporary Filename
     */
    virtual QString getFileName();

private:
    QList< QString > m_packages;
    QList< QUrl > m_repositories;

    QString m_tmpFileName;
};

#endif
