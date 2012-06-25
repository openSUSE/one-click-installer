#include "repository.h"

OCI::Repository::Repository()
{
}

QString OCI::Repository::recommended() const
{
	return m_recommended;
}

QString OCI::Repository::name() const
{
	return m_name;
}

QString OCI::Repository::summary() const
{
	return m_summary;
}

QString OCI::Repository::description() const
{
	return m_description;
}

QString OCI::Repository::url() const
{
	return m_url;
}

void OCI::Repository::setName( const QString& name )
{
	m_name = name;
}

void OCI::Repository::setRecommended( const QString& recommended )
{
	m_recommended = recommended;
}

void OCI::Repository::setSummary( const QString& summary )
{
	m_summary = summary;
}

void OCI::Repository::setDescription( const QString& description )
{
	m_description = description;
}

void OCI::Repository::setUrl( const QString& url )
{
	m_url = url;
}
