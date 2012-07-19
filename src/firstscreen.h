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
#include "packagedetails.h"
#include "fakebackend.h"
#include "packagebackend.h"
#include "ympparser.h"
#include "settings.h"
#include "package.h"
#include "repository.h"
#include "installscreen.h"
#include "details.h"
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
        Shows the Settings dialog
   */
    void showSettings();

   /**
        Triggers the installation when the Install software button is clicked
   */
    void performInstallation();

    /**
        More Details on untrusted repositories
    */
    void untrusedRepoDetails( QString link);

    /**
        Show Event
     */
    void showEvent( QShowEvent * s);

private:
    QLabel *m_warning;

    QPushButton *m_showSettings;
    QPushButton *m_cancel;
    QPushButton *m_install;

    PackageBackend *m_backend;

    QList< OCI::Package* > m_packages;
    QList< OCI::Repository* > m_repos;


    QString *m_tmpFileName;

    QSettings m_settings;
signals:
    void showNextScreen( int );
    void countChanged( int, int );
};

#endif
