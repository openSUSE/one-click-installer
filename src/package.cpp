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

void OCI::Package::setName( const QString & name )
{
    m_name = name;
}

void OCI::Package::setSummary( const QString & summary )
{
    m_summary = summary;
}

void OCI::Package::setDescription( const QString & description )
{
    m_description = description;
}
