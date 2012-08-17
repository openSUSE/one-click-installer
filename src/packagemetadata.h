#ifndef PACKAGEMETADATA_H
#define PACKAGEMETADATA_H

#include <QDebug>
#include <QProcess>
#include <QObject>
#include <QStringList>

class PackageMetadata : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief PackageMetadata
     * @param name : package name
     */
    PackageMetadata( QString name );

    /**
     * Return the size of the package
     */
    QString size();

    /**
     * Return the version of the package
     */
    QString version();

    /**
     * Start the subprocess to retrieve the data
     */
    void getData();
private:
    QString m_size;
    QString m_version;
    QString m_stdout;

    QString m_package;

    QProcess *m_process;
private slots:

    /**
     * Check if the subprocess started
     */
    void isStarted();

    /**
     * Process output when subprocess finishes
     */
    void isFinished( int );

signals:
    void finished( QString version, QString size );
};

#endif
