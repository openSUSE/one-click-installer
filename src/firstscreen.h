#ifndef FIRSTSCREEN_H
#define FIRSTSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QList>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QUuid>
#include <QSettings>
#include <klocalizedstring.h>
#include "packagedetails.h"
#include "fakebackend.h"
#include "packagebackend.h"
#include "ympparser.h"
#include "settings.h"
#include "package.h"
#include "repository.h"
#include "installscreen.h"
#include "repositorywidget.h"
#include "summary.h"

class MainWindow;

class FirstScreen : public QWidget
{
    Q_OBJECT
public:
   /**
        Default constructor taking the backend, and the filename as argument
   */
    FirstScreen( PackageBackend *backend, QString *tmpFileName, const QString& filename, QObject *parent = 0 );
    
private slots:

    /**
        Show Event
     */
    void showEvent( QShowEvent * s);
    /**
     *  Count selected packages 
     */
    void checkPackagesInstallableState();

private:
    PackageBackend *m_backend;

    QList< OCI::Package* > m_packages;
    QList< OCI::Repository* > m_repos;
    QString *m_tmpFileName;
    QLabel *m_warning;
    int m_untrustedSources;

signals:
    void showNextScreen( int );
    void countChanged( int, int );
    void sizeUpdated( QString size );
    void packageListInstallableStateToggled( bool );
};

#endif
