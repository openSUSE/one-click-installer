#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include "utils.h"
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
        Function to perform installation
     */
    void install();

    /**
        Add Repositories
     */
    static void addRepository();

    /**
     * Add packages
     */
    static void addPackage();
    
Q_SIGNALS:
    void hasConflicts();
    void displayProblemAndSolutions( QString, QStringList );
    void noConflicts();
    
public Q_SLOTS:
    void killBackend();
private:
    
    /**
     * Resolve package dependencies
     */
    void resolve();
    void resolve( const ResolverProblem& problem, ProblemSolutionList& todo );
    
    org::opensuse::oneclickinstaller *m_oci;
};

#endif
