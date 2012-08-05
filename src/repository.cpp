//      Copyright 2012 Saurabh Sood <saurabh@saurabh.geeko>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//      


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
