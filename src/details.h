#ifndef DETAILS_H
#define DETAILS_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "repository.h"
#include "package.h"
#include "packagebackend.h"

class Details : public QWidget
{
public:
    Details( PackageBackend *backend, OCI::Repository *repo, QObject *parent = 0 );
private:
    PackageBackend *m_backend;

    QLabel *m_url;
    QLabel *m_id;
    QLabel *m_fingerprint;
    QLabel *m_created;
    QLabel *m_expires;
    QLabel *m_summary;
};

#endif
