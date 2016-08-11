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
  
  // progress for downloading a resolvable
  class DownloadResolvableReportReceiver: public callback::ReceiveReport<repo::DownloadResolvableReport>
  {
  private:
      Resolvable::constPtr m_resolvablePtr;
      Url m_url;
      Pathname m_delta;
      ByteCount m_deltaSize;
      string m_labelApplyDelta;
      Pathname m_patch;
      ByteCount m_patchSize;
      unsigned m_packageCurrent;
      
  public:
      /**
       * Dowmload delta rpm:
       *	 - path below url reported on start()
       *	 - expected download size (0 if unknown)
       *	 - download is interruptable
       *	 - problems are just informal
       */
      virtual void startDeltaDownload( const Pathname& fileName, const ByteCount& downloadSize )
      {
	  m_delta = fileName;
	  m_deltaSize = downloadSize;
	  cout << "Retrieving delta: " << m_delta << ", " << m_deltaSize << endl;
      }
      
      virtual void problemDeltaDownload( const string& description )
      {
	  cout << description << endl;
      }
      
      /**
       * Apply delta rpm:
       *	 - local path of downloaded delta
       *	 - aplpy is not interruptable
       *	 - problems are just informal
       */
      virtual void startDeltaApply( const Pathname& filename )
      {
	  m_delta = filename.basename();
	  m_labelApplyDelta = m_delta.asString();
	  cout << "Applying Delta: " << m_delta;
      }
      
      virtual void progressDeltaApply( int value )
      {
	  cout << m_labelApplyDelta << " " << value << endl;
      }
      
      virtual void problemDeltaApply( const string& description )
      {
	  cout << description;
      }
      
      virtual void finishDeltaApply()
      {
	  cout << "finished applying " << m_labelApplyDelta;
      }
      
      virtual void start( Resolvable::constPtr resolvablePtr, const Url& url)
      {
	  m_resolvablePtr = resolvablePtr;
	  m_url = url;
	  m_packageCurrent = 0;
	  Package::constPtr currentPackage = asKind<Package>( m_resolvablePtr );
	  
	  // Complete information for a GUI object to present it to the user
	  // emitted each time a new resolvable is being downloaded.
	  // Equivalent zypper output is as follows - 
	  // Output: Retrieving package gnome-video-effects-0.4.1-3.9.noarch  (1/39),  67.3 KiB (192.6 KiB unpacked)
	  
	  /*emit resolvableData( m_resolvablePtr->name(),
			       m_resolvablePtr->kind().asString(),			       
			       m_resolvablePtr->edition().asString(),
			       m_resolvablePtr->arch().asString(),
			       ++m_packageCurrent,
			       currentPackage->downloadSize().asString(),
			       currentPackage->installSize().asString()); */
	  
	  // An rpm_download flag for OCIHelper? Awesome design by zypper programmers :)
	  // OCIHelper.runtimeData().rpm_download = true;	  
      }
      
      // Not needed. The progress will be reported by the DownloadProgressReportReceiver's progress method
      // virtual bool progress( int value, Resolvable::constPtr resolvablePtr ) { return true; }
      
      virtual Action problem( Resolvable::constPtr resolvablePtr, Error error, const string& description )
      {
	  cout << "error report" << endl;
	  // emit( error, description );
	  // set the rpm_download flag false
	  // OCIHelper::instance()->runtimeData().rpm_download = false;
	  return (Action) DownloadResolvableReport::ABORT;
      }
      
      // Detail information about the result of a performed pkgGpgCheck.
      // Not really needed unless you say otherwise. :)
      // virtual void pkgGpgCheck( const UserData& userData )  { }
      
      // Not needed as DownloadProgressReportReceiver is handling the progress.
      // virtual void finish( Resolvable::constPtr /* resolvablePtr */, Error error, const string& reason ) {}
  };
  
  // This is not needed for now - just keeping it around for future use
  class ProgressReportReceiver : public callback::ReceiveReport<ProgressReport>
  {
  public:
      virtual void start( const ProgressData& data )
      {
	  cout << "=========================" << endl;
	  cout << "Object Numeric Id: " << data.numericId() << endl;
	  cout << "Counter Name: " << data.name() << endl;
	  cout << "Report Alive: " << data.reportAlive() << endl;
	  cout << "=========================" << endl;
      }
      
      virtual bool progress( const ProgressData& data )
      {
	  cout << "In ProgressReportReceiver progress() ";
	  if (data.reportAlive())
	    cout << data.numericId() << data.name();
	  else
            cout << data.numericId() << " " << data.name() << " " << data.val();
	  return true;
      }
  };
  
  // Convenience class for progress output. Needed by InstallResolvableReportReceiver
  class ProgressBar : private base::NonCopyable
  {
  public:
      ProgressBar( const string& progressId_R, const string& label_R, unsigned current_R = 0, unsigned total_R = 0 )
	: m_error( indeterminate )
	, m_progressId( progressId_R )
      {
	  m_progress.name( label_R );
	  m_progress.sendTo( Print( *this ) );
      }
      
      ~ProgressBar()
      {
	  // suppress ProgressData final report
	  m_progress.noSend();
	  if ( indeterminate( m_error ) )
	      m_error = ( m_progress.reportValue() != 100 && m_progress.reportPercent() );
	  // emit the error
	  // emit ( m_progressId, m_progress.name(), m_error );
      }
      
      /** print(emit) the progress bar not waiting for a new trigger */
      void print()
      { 
	  // emit( m_progressId, outLabel( m_progress.name() ), m_progress.reportValue() );
      }
      
      /** \overload also change the progress bar label */
      void print( const string& label_R )
      { m_progress.name( label_R ); print(); }
      
      /** Indicate the error condition for the final progress bar */
      void error( TriBool error_R = true )
      { m_error = error_R; }
      
      /** \overload disambiguate */
      void error( bool error_R )
      { m_error = error_R; }
      
      /** \overload also change the progress data (bar) label */
      void error( const string& label_R )
      { m_progress.name( label_R ); error( true ); }
      
      /** \overload set the TriBool and change the progress bar label */
      void error( TriBool error_R, const string& label_R )
      { m_progress.name( label_R ); error( error_R ); }
      
  public:
      /** \name Access the embedded ProgressData object */
      //@{
      ProgressData * operator->()
      { return &m_progress; }
      
      const ProgressData * operator->() const
      { return &m_progress; }
      
      ProgressData & operator*()
      { return m_progress; }
      
      const ProgressData & operator*() const
      { return m_progress; }
      //@}
      
  private:
      /** ProgressData::ReceiverFnc printing to a ProgressBar */
      class Print
      {
      public:
	  Print( ProgressBar& bar_R ) : m_bar( &bar_R ) {}
	  bool operator()( const ProgressData& progress_R )
	  {
	      // emit the actual installation progress
	      // emit( m_bar->m_progressId, m_bar->outLabel( progress_R.name() ), progress_R.reportValue() );
	      return true;
	  }
      private:
	  ProgressBar *m_bar;
      };
      
      string outLabel( const string& msg_R ) const
      { return m_labelPrefix.empty() ? msg_R : m_labelPrefix + msg_R; }
  private:
      TriBool m_error;
      ProgressData m_progress;
      string m_progressId;
      string m_labelPrefix;
  };
  
  // progress for installing a resolvable
  class InstallResolvableReportReceiver : public callback::ReceiveReport<target::rpm::InstallResolvableReport>
  {
  public:
      virtual void start( Resolvable::constPtr resolvable )
      {
	  // just take these values for time being. 
	  unsigned rpm_pkg_current = 0; // = OCIHelper.runtimeData().rpm_pkg_current;
	  unsigned rpm_pkgs_total = 1;  // = OCIHelper.runtimeData().rpm_pkgs_total
	  cout << "Installing: " << resolvable->asString();
	  m_progress.reset( new ProgressBar( "install-resolvable",
					     resolvable->asString(),
					     ++rpm_pkg_current,
					     rpm_pkgs_total ) );
	  (*m_progress)->range( 100 );
      }
      
      virtual bool progress(int value, Resolvable::constPtr resolvable )
      {
	  if ( m_progress )
	      (*m_progress)->set( value );
	  return true;
      }
      
      virtual Action problem( Resolvable::constPtr resolvable, Error error, const string& description, RpmLevel /*unused*/)
      {
	  if ( m_progress ) {
	      (*m_progress).error();
	      m_progress.reset();
	  }
	  
	  cout << "Installation of " << resolvable->asString() << " failed" << endl;
	  // emit the problem
	  // emit problemEncountered( resolvable->asString(), description );
	  return (Action) ABORT;
      }
      
      virtual void finish( Resolvable::constPtr resolvable, Error error, const string& reason, RpmLevel /*unused */)
      {
	  if ( m_progress ) {
	      (*m_progress).error( error != NO_ERROR );
	      m_progress.reset();
	  }
	  
	  if ( error != NO_ERROR ) {
	      // quit the helper and OCI
	  }
	  else {
	      if ( !reason.empty() ) ;
		  //emit( "install-finish", reason);
	  }
      }
      
      virtual void reportend()
      { m_progress.reset(); }
  private:
      scoped_ptr<ProgressBar> m_progress;
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

class SourceCallbacks
{
public:
    SourceCallbacks()
    {
      m_downloadReport.connect();
      m_progressReport.connect();
    }

    ~SourceCallbacks()
    {
      m_downloadReport.disconnect();
      m_progressReport.disconnect();
    }
private:
    OCICallbacks::DownloadResolvableReportReceiver m_downloadReport;
    OCICallbacks::ProgressReportReceiver m_progressReport;
};
#endif
