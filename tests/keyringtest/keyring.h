#ifndef KEYRING_H
#define KEYRING_H

#include <stdlib.h>
#include <iostream>
#include <boost/format.hpp>
#include <string>
#include <zypp/ZYppCallbacks.h>
#include <zypp/KeyRing.h>
#include <zypp/Pathname.h>
#include <zypp/base/Logger.h>

namespace zypp{
struct KeyRingReceive : public zypp::callback::ReceiveReport<zypp::KeyRingReport>
{
    bool keyRingExists;
    
    virtual KeyRingReport::KeyTrust askUserToAcceptKey( const PublicKey &key, const zypp::KeyContext& context )
    {
        keyRingExists = false;
        const std::string& keyid = key.id();
        const std::string & keyname = key.name();
        const std::string fingerprint = key.fingerprint();

        if ( keyid.empty() || keyname.empty() || fingerprint.empty() ) {
            keyRingExists = true;
        }
        //Printing Information

        std::cout<<"Key ID: "<<keyid.c_str()<<std::endl;
        std::cout<<"Key Name: " <<keyname.c_str()<<std::endl;
        std::cout<<"Key Fingerprint: "<<fingerprint.c_str()<<std::endl;
        std::cout<<"Key Created: "<<key.created().asString().c_str()<<std::endl;
        std::cout<<"Key Expires: "<<key.expiresAsString().c_str()<<std::endl;
        return KeyRingReport::KEY_TRUST_AND_IMPORT;
    }

    bool exists(){
        return keyRingExists;
    }

};
/*
class KeyRingCallbacks{
    private:
        zypp::KeyRingReceive _keyRingReport;
    public:
        KeyRingCallbacks()
        {
            _keyRingReport.connect();
        }
        ~KeyRingCallbacks()
        {
            _keyRingReport.disconnect();
        }
        bool exists()
        {
			return _keyRingReport.exists();
		}
};*/
}

#endif
