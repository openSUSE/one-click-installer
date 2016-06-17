#ifndef KEYRINGCALLBACKS_H
#define KEYRINGCALLBACKS_H

#include <QDebug>
#include <keyring.h>

namespace zypp {
class KeyRingCallbacks
{
public:
    /**
     * Default Constructor
     */
    KeyRingCallbacks();

    /**
    * Destructor
    */
    ~KeyRingCallbacks();

    /**
    * Returns whether the Keyring exists or not
    */
    bool exists();

    /**
        Returns the KeyRing name
     */
     std::string name();

    /**
        Returns the KeyRing ID
     */
     std::string id();

    /**
        Returns the KeyRing
     */
     std::string fingerprint();

    /**
        Returns the Creation Date of the Fingerprint
     */
     std::string created();

    /**
        Returns the Expiry Date of the Fingerprint
     */
     std::string expires();
private:
    KeyRingReceive m_keyReport;
 
};
}
#endif
