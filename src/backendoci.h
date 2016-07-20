#ifndef BACKENDOCI_H
#define BACKENDOCI_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QUrl>
#include <list>
#include "packagebackend.h"

class BackendOCI : public PackageBackend
{
    Q_OBJECT
public:
    /**
        Default Constructor
     */
    BackendOCI( QString tempFileName, int ociID );

    /**
        Call the Backend Helper
     */
    void callBackendHelper();
    
protected:
    /**
     * Check if repo exists in the system repos or not
     */
    bool exists( const QString& repo );

signals:
    void checkForConflicts();
private:
    QProcess *m_process;
    int m_ociID;
    QString m_tempFileName;
};

#endif
