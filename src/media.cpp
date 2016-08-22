#include "media.h"

namespace OCICallbacks
{

  MediaChangeReport::Action MediaChangeReportReceiver::requestMedia(Url& url, 
								  unsigned int mediumNr,
								  MediaChangeReport::Error error,
								  const string& description,
								  const vector< string >& devices,
								  unsigned int& index)
  {
      cout << "Work needed to be done" << endl;
      return MediaChangeReport::ABORT;
  }

  bool AuthenticationReportReceiver::prompt(const Url& url, const string& description, media::AuthData* authData)
  {
      // I guess there's no need for this.
      return true;
  }  
} //namespace OCICallbacks
