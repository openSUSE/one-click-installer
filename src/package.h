#ifndef PACKAGE_H
#define PACKAGE_H

#include <QString>
#include <QDebug>

namespace OCI {
class Package
{
public:
    /**
    Constructor for class Representing Package
    */
    Package();

    /**
    Returns the name of the package
    */
    QString name() const;

    /**
    Returns the summary of the package
    */
    QString summary() const;

    /**
    Returns the description of the package
    */
    QString description() const;

    /**
     * @brief setName : set name of package
     * @param name : package name
     */
    void setName( const QString& name );

    /**
     * @brief setSummary : set package summary
     * @param summary : package summary
     */
    void setSummary( const QString& summary );

    /**
     * @brief setDescription : set package description
     * @param description : package description
     */
    void setDescription( const QString& description );
private:
        QString m_name;
        QString m_summary;
        QString m_description;
        bool m_status;
};
}

#endif
