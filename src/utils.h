#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string.h>

#include <zypp/RepoManager.h>
#include <zypp/RepoInfo.h>
#include <zypp/PoolItem.h>
#include <zypp/PoolQuery.h>
#include "keyring.h"

using namespace std;
using namespace zypp;

class ZypperUtils
{
private:
  /******************************* Member Declarations *******************************/
  static PoolItem mainObject;
  static RepoManager *repoManager;
  static RepoManagerOptions *rOpts;
  static KeyRingReceive _keyReport;
  
public:
  /******************************* Method Declarations *******************************/
  /**
   * Initialize Repositories 
   */
  static void initRepository( const string& repoUrl );
  
  /**
   * Refresh RepoManager to query meta data of a package
   */
  static void refreshRepoManager(const RepoInfo& temp);
  
  /**
   * Initialize RepoInfo. Needed to refresh RepoManager
   * RepoInfo contains all the information there is about a repository
   */
  static RepoInfo initRepoInfo( const string& url );
  
  /**
   * Executes a query to obtain meta-data 
   */
  static PoolItem queryMetadataForPackage( const string& packageName );
  
  /**
   * Returns PoolItem object to the user to display various attributes of a package
   */
  static PoolItem packageObject(const PoolQuery& q);
  /**
   * Returns KeyRing report
   */
  static KeyRingReceive keyReport();
};
#endif
