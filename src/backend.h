#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QList>
#include <QString>
#include "zyppinstall.h"
#include "oci_interface.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_CLASSINFO( "D-Bus Interface", "org.opensuse.OCIHelper" )
    
public:
    /**
        Default Constructor
     */
    Backend();

    /**
     * Starts the installation process
     */
    void install();
    
    /**
     * Add package 
     */
    static void addPackage( const QString& package );
Q_SIGNALS:
    void hasConflicts();
    void displayProblemAndSolutions( QString, QStringList );
    void noConflicts();
    
public Q_SLOTS:
    void killBackend();
    void applySolution( int );
private:
    
    /**
     * Resolve package dependencies
     */
    void resolve();
    void resolveConflicts();
    void resolve( const ResolverProblem& problem );
    
    ZYpp::Ptr m_zypp;
    ResolverProblem m_currentProblem;
    QList<ResolverProblem_Ptr> m_resolverProblemList;
    QList<ProblemSolution_Ptr> m_solutionsToTry;
    org::opensuse::oneclickinstaller *m_oci;
        
    static QStringList s_packages;
};

#endif
