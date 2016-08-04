#ifndef ZYPPINFO_H
#define ZYPPINFO_H


#include <zypp/ResKind.h>
#include <zypp/base/Algorithm.h>
#include <zypp/ZYppFactory.h>
#include <zypp/ResPool.h>
#include <zypp/PoolQuery.h>
#include <zypp/base/Easy.h>
#include "utils.h"

class Info 
{
private:
    string m_repositoryName;
    string m_packageName;
    string m_version;
    string m_arch;
    string m_vendor;
    string m_status;
    string m_installedSize;
    bool m_installed;
public:
    string repository() { return m_repositoryName; }
    string packageName() { return m_packageName; }
    string version() { return m_version; }
    string arch() { return m_arch; }
    string vendor() { return m_vendor; }
    bool isInstalled() { return m_installed; }
    string status() { return m_status; }
    string installedSize() { return m_installedSize; }

    void setRepositoryName( const string& repository ) { m_repositoryName = repository; }
    void setPackageName( const string& packageName ) { m_packageName = packageName; }
    void setVersion( const string& version ) { m_version = version; }
    void setArch( const string& arch ) { m_arch = arch; }
    void setVendor( const string& vendor ) { m_vendor = vendor; }
    void setInstalled( bool Instflag ) { m_installed = Instflag; }
    void setStatus( const string& status ) { m_status = status; }
    void setInstalledSize( const string& installedSize ) { m_installedSize = installedSize; }
};

class ZyppInfo
{
public:
    /**
     * Executes query to obtain meta-data
     */
    static Info queryMetadataForPackage( const string& packageName );
 
    /**
     * Returns KeyRing report
     */
    static KeyRingReceive keyReport();
private:
    /**
     * Returns PoolItem object to the user to display various attributes of a packageName
     */
    static Info packageObject( const PoolQuery& q );
};

#endif
