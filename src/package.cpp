#include "package.h"

OCI::Package::Package()
{
}

QString OCI::Package::name() const
{
    return m_name;
}

QString OCI::Package::summary() const
{
    return m_summary;
}

QString OCI::Package::description() const
{
    return m_description;
}

void OCI::Package::setName( const QString& name )
{
    m_name = name;
}

void OCI::Package::setSummary( const QString& summary )
{
    m_summary = summary;
}

void OCI::Package::setDescription( const QString& description )
{
    m_description = description;
}
