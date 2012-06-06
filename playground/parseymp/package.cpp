#include "package.h"

/*Package::Package(QString name,QString summary,QString description)
{
	m_name = name;
	m_summary = summary;
	m_description = description;
}*/

Package::Package()
{
}

QString
Package::name()
{
	return m_name;
}

QString
Package::summary()
{
	return m_summary;
}

QString
Package::description()
{
	return m_description;
}

void
Package::setName( QString name )
{
	m_name = name;
}

void
Package::setSummary( QString summary )
{
	m_summary = summary;
}

void
Package::setDescription( QString description )
{
	m_description = description;
}
