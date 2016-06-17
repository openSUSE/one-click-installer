#ifndef PACKAGEBACKEND_H
#define PACKAGEBACKEND_H

#include <QList>
#include <QUrl>
#include <QObject>

class PackageBackend : public QObject
{
    Q_OBJECT
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

    /**
        Call the Backend Helper
     */
    virtual void callBackendHelper() = 0;

    /**
        Set the Temporary Filename
     */
    virtual void setFileName( const QString& fileName );

    /**
        Get the Temporary Filename
     */
    virtual QString getFileName();

    /**
        Return true if repository exists
     */
    virtual bool exists( const QString& url ) = 0;

private:
    QList< QString > m_packages;
    QList< QUrl > m_repositories;

    QString m_tmpFileName;

signals:
    void installationStarted();
    void installationCompleted();
    void installationFailed();
};

#endif
