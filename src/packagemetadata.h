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
private:
    QString m_size;
    QString m_version;

    QProcess *m_process;
private slots:
    void isStarted();
};

#endif
