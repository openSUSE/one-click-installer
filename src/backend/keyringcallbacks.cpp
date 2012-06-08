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
	return _keyRingReport.exists();
}
