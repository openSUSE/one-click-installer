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

QString Repository::recommended()
{
	return m_recommended;
}

QString Repository::name()
{
	return m_name;
}

QString Repository::summary()
{
	return m_summary;
}

QString Repository::description()
{
	return m_description;
}

QString Repository::url()
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
