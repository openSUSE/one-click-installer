#ifndef PACKAGEDETAILS_H
#define PACKAGEDETAILS_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QHash>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMovie>
#include <klocalizedstring.h>
#include "package.h"
#include "packagemetadata.h"

class PackageDetails : public QWidget
{
    Q_OBJECT
public:

    /**
     * Constructor for Package Widget. Parameter is the index and Package Object
     */
    PackageDetails(OCI::Package *package, int packagecount, int count, QObject *parent = 0 );
    
    bool shouldBeInstalled() const;
private:
    bool m_descriptionVisible;

    QLabel *m_summary;
    QLabel *m_singlePackage;
    QLabel *m_version;
    QLabel *m_size;
    QLabel *m_description;
    QLabel *m_showDescription;
    QCheckBox *m_packageName;
    QMovie m_fetchingAnimation;

    PackageMetadata *meta;

private slots:

    /**
     * Show the Package Description Label
     */
    void showPackageDescription( QString link );

    /**
     * Update the version and size of the package
     */
    void dataChanged( QString version, QString size );

signals:
    void sizeUpdated( QString size );
    void installableStateToggled(bool);
};

#endif
