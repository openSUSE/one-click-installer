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

    /**
     * @brief PackageDetails
     * @param package : Package object
     * @param count : package index
     * @param parent
     */
    PackageDetails( OCI::Package *package, int count, QObject *parent = 0 );
private:
    bool m_descriptionVisible;

    QLabel *m_summary;
    QLabel *m_version;
    QLabel *m_size;
    QLabel *m_description;
    QLabel *m_showDescription;
    QCheckBox *m_packageName;

    PackageMetadata *meta;

private slots:

    /**
     * @brief showPackageDescription : show/hide description
     * @param link : link no
     */
    void showPackageDescription( QString link );

    /**
     * @brief dataChanged : update version and size
     * @param version : package version
     * @param size : package size
     */
    void dataChanged( QString version, QString size );

signals:
    void sizeUpdated( QString size );
};

#endif
