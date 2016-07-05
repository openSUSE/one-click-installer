#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string.h>

#include <zypp/RepoManager.h>
#include <zypp/RepoInfo.h>
#include <zypp/PoolItem.h>
#include <zypp/PoolQuery.h>
#include <zypp/ZYpp.h>
#include "keyring.h"

using namespace std;
using namespace zypp;

class ZypperUtils
{
private:
  /******************************* Member Declarations *******************************/
  static RepoManager *s_repoManager;
  static RepoManagerOptions *s_repoManagerOpts;
  static RepoInfo s_repoInfo;
  static KeyRingReceive s_keyReceiveReport;
  
  static ZYpp::Ptr s_zypp;
  
public:
  /**
   * Initialize Repositories 
   */
  static void initRepository( const string& repoUrl );
  
  /**
   * Refresh RepoManager to query meta data of a package
   */
  static void refreshRepoManager();
  
  /**
   * Initialize RepoInfo. Needed to refresh RepoManager
   * RepoInfo contains all the information there is about a repository
   */
  static void initRepoInfo( const string& repoUrl, const string& packageName );
  
  /**
   * Intialize RepoManager
   */
  static void initRepoManager( const string& repoUrl, const string& packageName = "temp" );
 
  /*********************************** ZYPPER INFO ***********************************/
  /**
   * Executes query to obtain meta-data 
   */
  static PoolItem queryMetadataForPackage( const string& packageName );
  
  /**
   * Returns PoolItem object to the user to display various attributes of a package
   */
  static PoolItem packageObject( const PoolQuery& q );
  
  /**
   * Returns KeyRing report
   */
  static KeyRingReceive keyReport();
  
  /********************************* ZYPPER INSTALL **********************************/
 
  /**
   * Marks packages for installation in pool
   */
  static void markPackagesForInstallation( const string& repoUrl, const string& packageName );
  
  /**
   * Resolves conflicts and dependencies. Returns true on success and false if problems exist
   */
  static bool resolveConflictsAndDependencies();
  
  /**
   * Commits changes to the system
   */
  static bool commitChanges();
 
  /**
   * Initialize Target - Loads installed packages to pool
   */
  static void initTarget( const Pathname& sysRoot ); 
  
  /**
   * Returns ZYpp::Ptr
   */
  static ZYpp::Ptr zyppPtr();
  
  static void makeNecessaryChangesToRunAsRoot( const Pathname& sysRoot );
  
};
#endif
