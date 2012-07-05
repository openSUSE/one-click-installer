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
        Add Repositories
     */
    void addRepositories();

protected:
    /**
        Check if repo exists in the Repository database or not
     */
    bool exists( std::string repo );
private:
    zypp::RepoManager *m_manager;
    zypp::ZYpp::Ptr *m_ptr;
    zypp::KeyRingCallbacks *m_keyRingManager;
};

#endif
