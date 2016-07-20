#ifndef BACKEND_H
#define BACKEND_H

#include <QDebug>
#include <QUrl>
#include <QObject>
#include <QProcess>
#include <zypp/RepoManager.h>
#include <zypp/base/Algorithm.h>
#include <zypp/ResFilters.h>
#include <zypp/ResStatus.h>
#include <zypp/ResPool.h>
#include <zypp/ResPool.h>
#include <zypp/target/rpm/RpmDb.h>
#include <zypp/target/TargetException.h>
#include <zypp/ZYppCommit.h>
#include <zypp/base/Regex.h>
#include <zypp/sat/WhatProvides.h>
#include <zypp/ZYppFactory.h>
#include <zypp/misc/DefaultLoadSystem.h>
#include "dbusadaptor.h"
#include <QtDBus/QDBusConnection>
#include <list>
#include "keyringcallbacks.h"
#include "packagebackend.h"

class Backend : public PackageBackend
{
    Q_OBJECT
public:
    /**
        Default Constructor
     */
    Backend();

    /**
        Function to perform installation
     */
    void install();

    /**
        Call the Backend Helper
     */
    void callBackendHelper();

    /**
        Return Error Code;
     */
    int errorCode();

    /**
        Add Repositories
     */
    void addRepositories();

protected:
    /**
        Check if repo exists in the Repository database or not
     */
    bool exists( const QString& repo );

signals:
    void checkForConflicts();
    
private slots:
    void finished( int v );
    void started();

private:
    zypp::RepoManager *m_manager;
    zypp::ZYpp::Ptr *m_ptr;
    zypp::KeyRingCallbacks *m_keyRingManager;
    int m_errorCode;

    DBusAdaptor *m_adaptor;

    QProcess *m_process;

public Q_SLOTS:
    void KillBackend();
};

#endif
