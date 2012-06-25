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
    Sets the name for the package
    */
    void setName( const QString& name );

    /**
    Sets the summary for the package
    */
    void setSummary( const QString& summary );

    /**
    Sets the description for the package
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
