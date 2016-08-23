#ifndef KEYRING_H
#define KEYRING_H

#include <boost/format.hpp>
#include <string>
#include <zypp/ZYppCallbacks.h>
#include <zypp/KeyRing.h>
#include <zypp/Pathname.h>
#include <zypp/base/Logger.h>
#include <QDebug>

using namespace zypp;
struct KeyRingReceive : public zypp::callback::ReceiveReport<zypp::KeyRingReport>
{
    bool m_keyRingExists;
    std::string m_id;
    std::string m_name;
    std::string m_fingerprint;
    std::string m_created;
    std::string m_expires;

   /**
   	Get the Keyring and Ask the user to accept it
    */ 
    virtual KeyRingReport::KeyTrust askUserToAcceptKey( const PublicKey &key, const zypp::KeyContext& context )
    {
        m_keyRingExists = false;
        m_id = key.id();
        m_name = key.name();
        m_fingerprint = key.fingerprint();
        if ( m_id.empty() || m_name.empty() || m_fingerprint.empty() ) {
            m_keyRingExists = true;
        }
        m_created = key.created().asString();
        m_expires = key.expires().asString();

        //Printing Information
        qDebug() << "Key ID: " <<  m_id.c_str();
        qDebug() << "Key Name: " <<  m_name.c_str();
        qDebug() << "Key Fingerprint: " << m_fingerprint.c_str();
        qDebug() << "Key Created: "<< m_created.c_str();
        qDebug() << "Key Expires: "<< m_expires.c_str();
	
        return KeyRingReport::KEY_TRUST_AND_IMPORT;
    }
};

#endif
