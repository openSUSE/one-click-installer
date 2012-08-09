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
    PackageMetadata( QString name );
    QString size();
    QString version();
    void getData();
private:
    QString m_size;
    QString m_version;
    QString m_stdout;

    QString m_package;

    QProcess *m_process;
private slots:
    void isStarted();
    void isFinished( int );
signals:
    void finished( QString version, QString size );
};

#endif
