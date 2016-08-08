/*------------------------------------------------------------------------------------*\
   ____                _____ _ _      _      _____           _        _ _           
  / __ \              / ____| (_)    | |    |_   _|         | |      | | |          
 | |  | |_ __   ___  | |    | |_  ___| | __   | |  _ __  ___| |_ __ _| | | ___ _ __ 
 | |  | | '_ \ / _ \ | |    | | |/ __| |/ /   | | | '_ \/ __| __/ _` | | |/ _ \ '__|
 | |__| | | | |  __/ | |____| | | (__|   <   _| |_| | | \__ \ || (_| | | |  __/ |   
  \____/|_| |_|\___|  \_____|_|_|\___|_|\_\ |_____|_| |_|___/\__\__,_|_|_|\___|_|   
                                                                                    
\*------------------------------------------------------------------------------------*/
                                                                                    
#ifndef OCI_MEDIA_CALLBACKS_H
#define OCI_MEDIA_CALLBACKS_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ctime>

#include <zypp/ZYppCallbacks.h>
#include <zypp/base/Logger.h>
#include <zypp/Pathname.h>
#include <zypp/Url.h>

#define REPEAT_LIMIT 3

using namespace std;
using namespace zypp;
using media::MediaChangeReport;
using media::DownloadProgressReport;

namespace OCICallbacks
{
  class RepeatCounter 
  {
  public:
      RepeatCounter() : m_counter( 0 ) {}
      bool counterOverrun( const Url& url )
      {
	  if ( m_url == url ) {
	      if ( ++m_counter >= REPEAT_LIMIT ) {
		  m_counter = 0;
		  return true;
	      }
	  }
	  else {
	      m_url = url;
	      m_counter = 0;
	  }
	  return false;
      }
  private:
      unsigned m_counter;
      Url m_url;
  };
  
  class MediaChangeReportReceiver : public callback::ReceiveReport<media::MediaChangeReport>
  {
  public:
      virtual MediaChangeReport::Action requestMedia( Url& url, 
						      unsigned mediumNr,
						      MediaChangeReport::Error error,
						      const string& description,
						      const vector<string>& devices,
						      unsigned& index);
  private:
      RepeatCounter repeatCounter;
  };
  
  // progress for downloading a file
  class DownloadProgressReportReceiver : public callback::ReceiveReport<media::DownloadProgressReport>
  {
  public:
      virtual void start( const Url& url, Pathname localFile )
      {
	  m_lastReported = time( NULL );
	  m_lastDrateAvg = -1;
      }
      
      /**
       * Download progress
       * 
       * value        Percentage value.
       * file         File URI.
       * drateAvg     Average download rate so far. -1 if unknown.
       * drateNow     Current download (cca last 1 sec). -1 if unknown.
       * 
       * TODO implement OCI::exitRequested()
       * TODO return false on SIGINT
       */
      virtual bool progress( int value, const Url& uri, double drateAvg, double drateNow )
      {
	  time_t now = time( NULL );
	  if ( now > m_lastReported )
	      m_lastReported = now;
	  else
	      return true;
	  
	  /* 
	   * 	if ( OCIHelper::exitRequested() )
	   * 	   return false;
	   */
	  
	  /* emit the signal here to OCI?
	   * It will update the progress bar (GUI) based on value (percentage downloaded)
	   * update the speed based on drateNow in the header - internet speed */
	  
	  // emit downloadProgress( url, value, (long) drateNow )
	  m_lastDrateAvg = drateAvg;
	  return true;
      }
      
      virtual DownloadProgressReport::Action problem( const Url& uri, DownloadProgressReport::Error error, const string& description )
      {
	  // emit the error, and its description to the OCI
	  // emit problemEncountered( description );
	  return DownloadProgressReport::ABORT;
      }
      
      virtual void finish( const Url& uri, Error error, const string& reason )
      {
	  // Reports end of a download
	  // error != NO_ERROR --->> did the download finish with error?
	  // emit successFlag( uri, m_lastDrateAvg, error != NO_ERROR );
      }
  private:
      time_t m_lastReported;
      double m_lastDrateAvg;
  };
  
  class AuthenticationReportReceiver : public callback::ReceiveReport<media::AuthenticationReport>
  {
      virtual bool prompt( const Url& url,
			   const string& description,
			   media::AuthData* authData );
  };
} // namespace OCICallbacks

class MediaCallbacks
{
public:
    MediaCallbacks()
    {
	m_mediaChangeReport.connect();
	m_mediaDownloadReport.connect();
	m_mediaAuthenticationReport.connect();
    }
    
    ~MediaCallbacks()
    {
	m_mediaChangeReport.disconnect();
	m_mediaDownloadReport.disconnect();
	m_mediaAuthenticationReport.disconnect();
    }
private:
    OCICallbacks::MediaChangeReportReceiver m_mediaChangeReport;
    OCICallbacks::DownloadProgressReportReceiver m_mediaDownloadReport;
    OCICallbacks::AuthenticationReportReceiver m_mediaAuthenticationReport;
};
#endif
