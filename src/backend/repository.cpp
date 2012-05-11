#include "repository.h"

/*Repository::Repository(QString recommended,QString name,QString summary,QString description,QString url)
{
	m_recommended = recommended;
	m_name = name;
	m_summary = summary;
	m_description = description;
	m_url = url;
}*/

Repository::Repository()
{
}

QString Repository::recommended() const
{
	return m_recommended;
}

QString Repository::name() const
{
	return m_name;
}

QString Repository::summary() const
{
	return m_summary;
}

QString Repository::description() const
{
	return m_description;
}

QString Repository::url() const
{
	return m_url;
}

void Repository::setName(QString name)
{
	m_name = name;
}

void Repository::setRecommended(QString recommended)
{
	m_recommended = recommended;
}

void Repository::setSummary(QString summary)
{
	m_summary = summary;
}

void Repository::setDescription(QString description)
{
	m_description = description;
}

void Repository::setUrl(QString url)
{
	m_url = url;
}
