/***********************************************************************************
 *  One Click Installer makes it easy for users to install software, no matter
 *  where that software is located.
 *
 *  Copyright (C) 2016  Shalom <shalomray7@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************************
 *  This program's developed as part of GSoC - 2016
 *  Project: One Click Installer
 *  Mentors: Antonio Larrosa, and Cornelius Schumacher
 *  Organization: OpenSUSE
 *  Previous Contributor: Saurabh Sood
 ***********************************************************************************/

#include "keyringcallbacks.h"
#include "utils.h"

zypp::KeyRingCallbacks::KeyRingCallbacks()
{
    m_keyReport = ZypperUtils::keyReport();
    m_keyReport.connect();
}

zypp::KeyRingCallbacks::~KeyRingCallbacks()
{
    m_keyReport.disconnect();
}

bool zypp::KeyRingCallbacks::exists()
{
    return m_keyReport.m_keyRingExists;
}

std::string zypp::KeyRingCallbacks::name(){
    return m_keyReport.m_name;
}

std::string zypp::KeyRingCallbacks::id(){
    return m_keyReport.m_id;
}

std::string zypp::KeyRingCallbacks::fingerprint(){
    return m_keyReport.m_fingerprint;
}

std::string zypp::KeyRingCallbacks::created(){
    return m_keyReport.m_created;
}

std::string zypp::KeyRingCallbacks::expires(){
    return m_keyReport.m_expires;
}
