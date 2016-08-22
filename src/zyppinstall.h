#ifndef ZYPPINSTALL_H
#define ZYPPINSTALL_H

#include <zypp/ZYpp.h>
#include <string.h>

using namespace std;
using namespace zypp;

class ZyppInstall
{
public:
  /**
   * Marks packages for installation in pool
   */
  static void markPackagesForInstallation( const string & packageName );
  
  /**
   * Resolves conflicts and dependencies. Returns true on success and false if problems exist
   */
  static bool resolveConflictsAndDependencies();
  
  /**
   * Commits changes to the system
   */
  static bool commitChanges();
 
  /**
   * Returns ZYpp::Ptr 
   */
  static ZYpp::Ptr zyppPtr();
private:
  /**
   * Initialize Target - Loads installed packages to pool
   */
  static void initTarget( const Pathname& sysRoot ); 
private:
  static ZYpp::Ptr s_zypp;
};



#endif
