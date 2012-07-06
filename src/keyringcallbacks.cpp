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
