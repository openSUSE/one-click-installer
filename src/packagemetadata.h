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
     * Default ctor
     */
    PackageMetadata();

    /**
     * Return the size of the package
     */
    QString size();

    /**
     * Return the version of the package
     */
    QString version();
    
    /**
     * Query Metadata Information
     */
    void getData( const QString& packageName );
private:
      
    QString m_size;
    QString m_version;
    
private slots:
  
    /**
     * Emit finished signal
     */
    void isFinished();

signals:
    void finished( QString version, QString size );
};

#endif
