#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class Backend : public QObject
{
    Q_OBJECT
    Q_CLASSINFO( "D-Bus Interface", "org.opensuse.oneclickinstaller" )
    
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
    void displayProblem( QString );
    void displaySolution( QString );
};

#endif
