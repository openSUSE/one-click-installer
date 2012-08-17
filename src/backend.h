#ifndef BACKEND_H
#define BACKEND_H

#include <QDebug>
#include <QUrl>
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
#include <list>
#include "keyringcallbacks.h"
#include "packagebackend.h"

class Backend : public PackageBackend
{
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

protected:
    /**
     * @brief returns true if repository exists in database
     * @param repo : the url of the repository
     * @return
     */
    bool exists( std::string repo );

    /**
        Add Repositories
     */
    void addRepositories();

private:
    zypp::RepoManager *m_manager;
    zypp::ZYpp::Ptr *m_ptr;
    zypp::KeyRingCallbacks *m_keyRingManager;
    int m_errorCode;
};

#endif
