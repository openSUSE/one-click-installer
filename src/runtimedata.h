#ifndef RUNTIMEDATA_H
#define RUNTIMEDATA_H

#include <QStringList>
/**
 * Extend it to hold packages and repos as well - to simplify installation
 */
class RuntimeData
{
public:
    RuntimeData()
    : m_commitPackagesTotal( 0 )
    , m_commitPackageCurrent( 0 )
    , m_rpmPackagesTotal( 0 )
    , m_rpmPackageCurrent( 0 )
    , m_enteredCommit( false )
    , m_waitingForInputFromKMsgBox( false )
    , m_rpmDownload( false )
    {}
    
    // setters
    void setCommitPackagesTotal( unsigned value )
    { m_commitPackagesTotal = value; }
    void setCommitPackageCurrent( unsigned value )
    { m_commitPackageCurrent = value; }
    void setRpmPackagesTotal( unsigned value )
    { m_rpmPackagesTotal = value; }
    void setRpmPackageCurrent( unsigned value )
    { m_rpmPackageCurrent = value; }
    void setEnteredCommit( bool flag )
    { m_enteredCommit = flag; }
    void setWaitingForInputFromKMsgBox( bool flag )
    { m_waitingForInputFromKMsgBox = flag; }
    void setRpmDownload( bool flag )
    { m_rpmDownload = flag; }
    
    // getters
    int commitPackagesTotal()
    { return m_commitPackagesTotal; }
    int commitPackageCurrent()
    { return m_commitPackageCurrent; }
    int rpmPackagesTotal()
    { return m_rpmPackagesTotal; }
    int rpmPackageCurrent()
    { return m_rpmPackageCurrent; }
    bool enteredCommit()
    { return m_enteredCommit; }
    bool waitingForInputFromKMsgBox()
    { return m_waitingForInputFromKMsgBox; }
    bool rpmDownload()
    { return m_rpmDownload; }
    QStringList installationLog()
    { return m_installationLog; }
    
    // instance
    static RuntimeData* instance()
    {
      if ( !s_instance )
	  s_instance = new RuntimeData();
      return s_instance;
    }
    
    void writeInfo( const QString & info )
    {
	qDebug() << info;
	m_installationLog << info;
    }
    
private:
    unsigned m_commitPackagesTotal;
    unsigned m_commitPackageCurrent;
    unsigned m_rpmPackagesTotal;
    unsigned m_rpmPackageCurrent;
    bool m_enteredCommit;
    bool m_waitingForInputFromKMsgBox;
    bool m_rpmDownload;
    QStringList m_installationLog;
    static RuntimeData *s_instance;
};
#endif
