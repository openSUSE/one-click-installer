#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QtDBus/QDBusConnection>

class Backend : public QObject
{
    
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

private:
    
};

#endif
