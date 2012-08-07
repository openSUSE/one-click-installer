#include "packagemetadata.h"

PackageMetadata::PackageMetadata( QString name )
{
    QString processName = "zypper info " + name;
    qDebug() << processName;
    m_process = new QProcess;
    QObject::connect( m_process, SIGNAL( started() ), this, SLOT( isStarted() ) );
    m_process->start( processName );

    m_process->waitForFinished( -1 );

    QString stdout = m_process->readAllStandardOutput();

    QStringList details = stdout.split( "\n" );

    foreach( QString temp, details ) {
        if( temp.contains("Version") ) {
            m_version = temp.split( " " ).at( 1 );
        } else if( temp.contains( "Installed Size" ) ) {
            QStringList size = temp.split( " " );
            m_size = size.at( 2 ) + " " + size.at( 3 );
        }
    }

    qDebug() << m_size;
    qDebug() << m_version;
}

QString PackageMetadata::size()
{
    return m_size;
}

QString PackageMetadata::version()
{
    return m_version;
}

void PackageMetadata::isStarted()
{
    qDebug() << "zypper called";
}
