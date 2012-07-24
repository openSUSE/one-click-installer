#ifndef REPOSITORYMETADATA_H
#define REPOSITORYMETADATA_H

#include <iostream>
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
#include <string>
#include "keyringcallbacks.h"

class RepositoryMetadata
{
public:
    RepositoryMetadata( std::string url );
    std::string fingerprint();
    std::string created();
    std::string expires();
    std::string id();
private:
    zypp::KeyRingCallbacks m_keyRing;
};

#endif
