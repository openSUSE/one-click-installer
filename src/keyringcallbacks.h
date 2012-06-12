#ifndef KEYRINGCALLBACKS_H
#define KEYRINGCALLBACKS_H

#include "keyring.h"

namespace zypp{
class KeyRingCallbacks{
    private:
        zypp::KeyRingReceive _keyRingReport;
    public:
        KeyRingCallbacks();
        ~KeyRingCallbacks();
        bool exists();
};
}

#endif
