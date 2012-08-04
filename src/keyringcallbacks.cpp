//      keyringcallbacks.cpp
//      
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


#include "keyringcallbacks.h"

zypp::KeyRingCallbacks::KeyRingCallbacks()
{
	_keyRingReport.connect();
}

zypp::KeyRingCallbacks::~KeyRingCallbacks()
{
	_keyRingReport.disconnect();
}

bool zypp::KeyRingCallbacks::exists()
{
    return _keyRingReport.m_keyRingExists;
}

std::string zypp::KeyRingCallbacks::name(){
    return _keyRingReport.m_name;
}

std::string zypp::KeyRingCallbacks::id(){
    return _keyRingReport.m_id;
}

std::string zypp::KeyRingCallbacks::fingerprint(){
    return _keyRingReport.m_fingerprint;
}

std::string zypp::KeyRingCallbacks::created(){
    return _keyRingReport.m_created;
}

std::string zypp::KeyRingCallbacks::expires(){
    return _keyRingReport.m_expires;
}
