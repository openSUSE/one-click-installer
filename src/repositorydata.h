#include <string>
#include <zypp/Capabilities.h>
#include <zypp/sat/WhatProvides.h>
#include <zypp/sat/Solvable.h>
#include <zypp/RepoManager.h>
#include <zypp/misc/DefaultLoadSystem.h>
#include <zypp/ZYppFactory.h>
#include <zypp/ui/Selectable.h>
#include <zypp/ResObject.h>
#include <zypp/ResKind.h>
#include <zypp/base/LogTools.h>
#include <zypp/base/LogControl.h>
#include <zypp/ByteCount.h>
#include "keyringcallbacks.h"

class RepositoryData
{
public:

    /**
     * Default Constructor
     */
    RepositoryData();

    /**
     * @brief setBaseUrl : set repo base url
     * @param url : repo url
     */
    void setBaseUrl( std::string url );

    /**
     * @brief setAlias : set repo alias
     * @param alias : repo alias
     */
    void setAlias( std::string alias );

    /**
     * Return the KeyRing
     */
    zypp::KeyRingCallbacks keyring();

    /**
     * Refresh Repository Metadata
     */
    void refreshMetadata();

private:
    zypp::RepoManagerOptions *m_options;
    zypp::RepoInfo m_repo;
    zypp::RepoManager *m_repoManager;
    zypp::KeyRingCallbacks m_keyring;

    std::string m_baseUrl;
    std::string m_alias;
};
