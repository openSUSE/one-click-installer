#ifndef DETAILS_H
#define DETAILS_H

#include <QWidget>
#include "PackageBackend.h"
#include <QLabel>
#include <QVBoxLayout>

class Details : public QWidget
{
public:
    Details( PackageBackend *backend, QObject *parent = 0 );
private:
    PackageBackend *m_backend;
    QLabel *m_url;
    QLabel *m_id;
    QLabel *m_fingerprint;
    QLabel *m_created;
    QLabel *m_expires;
};

#endif
