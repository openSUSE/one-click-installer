#ifndef PACKAGEMETADATA_H
#define PACKAGEMETADATA_H

#include <QDebug>
#include <QObject>
#include "repository.h"

using namespace std;

class PackageMetadata : public QObject
{
    Q_OBJECT
public:

    /**
     * Construct the Object using the name of the package
     */
    PackageMetadata( const QString& packageName );

    /**
     * Return the size of the package
     */
    QString size();

    /**
     * Return the version of the package
     */
    QString version();
    
private:
      
    QString m_size;
    QString m_version;
private slots:

    /**
     * Process output when subprocess finishes
     */
    void isFinished();

signals:
    void finished( QString version, QString size );
};

#endif
