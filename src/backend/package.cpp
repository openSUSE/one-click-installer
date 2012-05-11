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

QString Package::name() const
{
	return m_name;
}

QString Package::summary() const
{
	return m_summary;
}

QString Package::description() const
{
	return m_description;
}

void Package::setName(const QString& name)
{
	m_name = name;
}

void Package::setSummary(const QString& summary)
{
	m_summary = summary;
}

void Package::setDescription(const QString& description)
{
	m_description = description;
}
