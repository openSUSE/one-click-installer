#ifndef PACKAGEDETAILS_H
#define PACKAGEDETAILS_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QHash>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "package.h"
#include "packagemetadata.h"

class PackageDetails : public QWidget
{
    Q_OBJECT
public:
    PackageDetails( OCI::Package *package, int count, QObject *parent = 0 );
    void showEvent( QShowEvent *e );
private:
    bool m_descriptionVisible;

    QLabel *m_summary;
    QLabel *m_version;
    QLabel *m_size;
    QLabel *m_description;
    QLabel *m_showDescription;
    QCheckBox *m_packageName;

private slots:
    void showPackageDescription( QString link );
};

#endif
