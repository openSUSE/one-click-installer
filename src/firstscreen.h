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
    FirstScreen( PackageBackend *backend, QString *tmpFileName, bool fakeRequested, QWidget* stageWidget, const QString& filename, QObject *parent = 0 );
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
        Shows Details of Repositories
    */
    void showDetails( QString link );

private:
    QLabel *m_warning;

    QPushButton *m_settings;
    QPushButton *m_cancel;
    QPushButton *m_install;

    PackageBackend *m_backend;
    QWidget *m_stageWidget;

    QList< QVBoxLayout* > m_repoLayouts;

    QList< bool > m_visible;
    QList< OCI::Package* > m_packages;
    QList< OCI::Repository* > m_repos;

    QHash< int, QWidget* > m_details;
    QHash< int, QLabel*> m_detailsLabels;

    QString *m_tmpFileName;
    bool m_fakeRequested;
};

#endif
