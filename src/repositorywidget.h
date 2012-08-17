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
     * @brief RepositoryWidget
     * @param backend : backend pointer
     * @param index : repository index
     * @param repo : Repository object
     * @param parent
     */
    RepositoryWidget(PackageBackend *backend, int index, OCI::Repository *repo, QObject *parent = 0 );

private slots:

    /**
     * @brief showDetails : show repository details
     * @param link : link no
     */
    void showDetails( QString link );

private:
    PackageBackend *m_backend;

    QLabel *m_name;
    QLabel *m_showDetails;
    QLabel *m_url;
    QLabel *m_id;
    QLabel *m_fingerprint;
    QLabel *m_created;
    QLabel *m_expires;
    QLabel *m_summary;
    QSettings m_settings;

    OCI::Repository *m_repo;

    RepositoryMetadata *m_meta;

    bool m_visible;

    bool m_detailsVisible;
};

#endif
