#ifndef KEYRINGCALLBACKS_H
#define KEYRINGCALLBACKS_H

#include "keyring.h"

namespace zypp{
class KeyRingCallbacks{
    private:
        zypp::KeyRingReceive _keyRingReport;
    public:
        /**
		Default Constructor
	 */
	KeyRingCallbacks();

	/**
		Destructor
	 */
        ~KeyRingCallbacks();

	/**
		Returns whether the Keyring exists or not
	 */
        bool exists();
};
}

#endif
