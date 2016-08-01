#ifndef REPOSITORYWIDGET_H
#define REPOSITORYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSettings>
#include "repository.h"
#include "package.h"
#include "packagebackend.h"
#include "repositorymetadata.h"

class RepositoryWidget : public QWidget
{
    Q_OBJECT
public:

    /**
     * Construct widget with index, Repository Object and Backend object
     */
    RepositoryWidget( PackageBackend *backend, int index, OCI::Repository *repo, QObject *parent = 0 );

private slots:

    /**
     * Show Repository Details
     */
    void showDetails( QString link );

private:
   
    QLabel *m_name;
    QLabel *m_showDetails;
    QLabel *m_url;
    QLabel *m_id;
    QLabel *m_fingerprint;
    QLabel *m_created;
    QLabel *m_expires;
    QSettings m_settings;

    OCI::Repository *m_repo;
    PackageBackend *m_backend;
    RepositoryMetadata *m_meta;

    bool m_visible;
    bool m_detailsVisible;
};

#endif
