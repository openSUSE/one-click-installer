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
#include <zypp/sat/Queue.h>
#include <zypp/sat/FileConflicts.h>
#include <zypp/base/Easy.h>


#include <QString>
#include <QDebug>
#include <KF5/KWidgetsAddons/kmessagebox.h> //for file conflicts
#include "callbacks.h" // emit values to the OCI
#include "runtimedata.h"

#define REPEAT_LIMIT 3

using namespace std;
using namespace zypp;
using media::MediaChangeReport;
using media::DownloadProgressReport;

static Callbacks s_toOCI;

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
      virtual MediaChangeReport::Action requestMedia( Url & url, 
						      unsigned mediumNr,
						      MediaChangeReport::Error error,
						      const string & description,
						      const vector<string> & devices,
						      unsigned & index);
  private:
      RepeatCounter repeatCounter;
  };
  
  // progress for downloading a file
  class DownloadProgressReportReceiver : public callback::ReceiveReport<media::DownloadProgressReport>
  {
  public:
      virtual void start( const Url & url, Pathname localFile )
      {
	  m_lastReported = time( NULL );
	  m_lastDrateAvg = -1;
	  
	  QString info( QString::fromStdString( "Retrieving: " + Pathname( url.getPathName() ).basename() ) );
	  s_toOCI.emitStartProgress( info );
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
      virtual bool progress( int value, const Url & uri, double drateAvg, double drateNow )
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
	  
	  s_toOCI.emitProgress( value );
	  m_lastDrateAvg = drateAvg;
	  return true;
      }
      
      virtual DownloadProgressReport::Action problem( const Url & uri, DownloadProgressReport::Error error, const string & description )
      {
	  // emit the error, and its description to the OCI
	  // emit problemEncountered( description );
	  return DownloadProgressReport::ABORT;
      }
      
      virtual void finish( const Url & uri, Error error, const string & reason )
      {
	  // Reports end of a download
	  // error != NO_ERROR --->> did the download finish with error?
	 
	  QString info( QString::fromStdString( "Retrieved: " + Pathname( uri.getPathName() ).basename() ) );
	  s_toOCI.emitFinishProgress( info, error != NO_ERROR );
      }
  
  private:
      time_t m_lastReported;
      double m_lastDrateAvg;
  };
  
  class AuthenticationReportReceiver : public callback::ReceiveReport<media::AuthenticationReport>
  {
      virtual bool prompt( const Url & url,
			   const string & description,
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
      
  public:
      /**
       * Download delta rpm:
       *	 - path below url reported on start()
       *	 - expected download size (0 if unknown)
       *	 - download is interruptable
       *	 - problems are just informal
       */
      virtual void startDeltaDownload( const Pathname & fileName, const ByteCount & downloadSize )
      {
	  m_delta = fileName;
	  m_deltaSize = downloadSize;
	  qDebug() << "Retrieving delta: " << m_delta.c_str() << ", " << m_deltaSize.asString().c_str();
	  QString info( QString::fromStdString( "Retrieving Delta: " + fileName.asString() ) );
	  s_toOCI.emitStartResolvable( info );
      }
      
      virtual void problemDeltaDownload( const string & description )
      {
	  qDebug() << description.c_str() ;
      }
      
      /**
       * Apply delta rpm:
       *	 - local path of downloaded delta
       *	 - apply is not interruptable
       *	 - problems are just informal
       */
      virtual void startDeltaApply( const Pathname & filename )
      {
	  m_delta = filename.basename();
	  m_labelApplyDelta = m_delta.asString();
	  qDebug() << "Applying Delta: " << m_delta.c_str();
	  QString info( QString::fromStdString( "Applying Delta: " + filename.asString() ) );
	  s_toOCI.emitStartResolvable( info );
      }
      
      virtual void progressDeltaApply( int value )
      {
	  qDebug() << m_labelApplyDelta.c_str() << " " << value ;
	  s_toOCI.emitProgress( value );
      }
      
      virtual void problemDeltaApply( const string & description )
      {
	  qDebug() << description.c_str();
      }
      
      virtual void finishDeltaApply()
      {
	  qDebug() << "finished applying " << m_labelApplyDelta.c_str();
	  s_toOCI.emitFinishResolvable( QString::fromStdString( "Finished Applying" ), true );
      }
      
      virtual void start( Resolvable::constPtr resolvablePtr, const Url & url)
      {
	  m_resolvablePtr = resolvablePtr;
	  m_url = url;
	  Package::constPtr currentPackage = asKind<Package>( m_resolvablePtr );
	  
	  /*
	   * - Complete information for a GUI object to present it to the user
	   * - emitted each time a new resolvable is being downloaded.
	   * - Equivalent zypper output is as follows - 
	   * - Output: (1/39) Retrieving package gnome-video-effects-0.4.1-3.9.noarch
	   */
	  
	  // An rpm_download flag for OCIHelper? Awesome design by zypper programmers :)
	  // OCIHelper.runtimeData().rpm_download = true;
	  
	  //set current package
	  RuntimeData::instance()->setCommitPackageCurrent( RuntimeData::instance()->commitPackageCurrent() + 1 );
	  s_toOCI.emitStartResolvable( QString::fromStdString( getStartDownloadResolvableInfo( resolvablePtr ) ) );
      }
      
      // Not needed. The progress will be reported by the DownloadProgressReportReceiver's progress method
      // virtual bool progress( int value, Resolvable::constPtr resolvablePtr ) { return true; }
      
      virtual Action problem( Resolvable::constPtr resolvablePtr, Error error, const string & description )
      {
	  qDebug() << "error report" ;
	  // emit( error, description );
	  // set the rpm_download flag false
	  // OCIHelper::instance()->runtimeData().rpm_download = false;
	  return (Action) DownloadResolvableReport::ABORT;
      }
      
      // Detail information about the result of a performed pkgGpgCheck.
      // Not really needed unless you say otherwise. :)
      // virtual void pkgGpgCheck( const UserData& userData )  { }
      
      // Although DownloadProgressReportReceiver is handling the progress, report download size and unpacked size
      virtual void finish( Resolvable::constPtr resolvablePtr , Error error, const string & reason )
      {
	  s_toOCI.emitFinishResolvable( QString::fromStdString( getFinishDownloadResolvableInfo( resolvablePtr ) ), error != NO_ERROR );
      }
      
      string getStartDownloadResolvableInfo( Resolvable::constPtr & resolvablePtr )
      {
	  // #006325 -> Dark Green
	  string line( str::Format( "<font color=#006325> ( %s / %s ) </font> <b>Retrieving %s:</b> </b><font color=\"Red\">%s-%s.%s</font>" )
				    % RuntimeData::instance()->commitPackageCurrent()
				    % RuntimeData::instance()->commitPackagesTotal()
				    % resolvablePtr->kind()
				    % resolvablePtr->name()
				    % resolvablePtr->edition()
				    % resolvablePtr->arch() );
				    
	  return line;	    
      }
      
      string getFinishDownloadResolvableInfo( Resolvable::constPtr & resolvablePtr)
      {
	  // #14148c -> Dark Blue; 
	  string line( str::Format( "Download Size:<font color= #14148c> %s </font> Unpacked Size:<font color=\"Purple\"> %s </font><br>" )
				    % resolvablePtr->downloadSize()
				    % resolvablePtr->installSize() );
	  return line;
      }
  };
  
  // This is not needed for now - just keeping it around for future use
  class ProgressReportReceiver : public callback::ReceiveReport<ProgressReport>
  {
  public:
      virtual void start( const ProgressData & data )
      {
	  qDebug() << "=========================" ;
	  qDebug() << "Object Numeric Id: " << data.numericId() ;
	  qDebug() << "Counter Name: " << data.name().c_str() ;
	  qDebug() << "Report Alive: " << data.reportAlive() ;
	  qDebug() << "=========================" ;
      }
      
      virtual bool progress( const ProgressData & data )
      {
	  qDebug() << "In ProgressReportReceiver progress() ";
	  if (data.reportAlive())
	    qDebug() << data.numericId() << data.name().c_str();
	  else
            qDebug() << data.numericId() << " " << data.name().c_str() << " " << data.val();
	  return true;
      }
  };
  
  // Convenience class for progress output. Needed by InstallResolvableReportReceiver
  class ProgressBar : private base::NonCopyable
  {
  public:
      ProgressBar( const string & progressId_R, const string & label_R, unsigned current_R = 0, unsigned total_R = 0 )
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
      }
      
      /** print(emit) the progress bar not waiting for a new trigger */
      void print()
      { 
	   //s_toOCI.emitStartProgress( m_progressId, outLabel( m_progress.name() ), m_progress.reportValue() );
      }
      
      /** \overload also change the progress bar label */
      void print( const string & label_R )
      { m_progress.name( label_R ); print(); }
      
      /** Indicate the error condition for the final progress bar */
      void error( TriBool error_R = true )
      { m_error = error_R; }
      
      /** \overload disambiguate */
      void error( bool error_R )
      { m_error = error_R; }
      
      /** \overload also change the progress data (bar) label */
      void error( const string & label_R )
      { m_progress.name( label_R ); error( true ); }
      
      /** \overload set the TriBool and change the progress bar label */
      void error( TriBool error_R, const string & label_R )
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
	  Print( ProgressBar & bar_R ) : m_bar( &bar_R ) {}
	  bool operator()( const ProgressData & progress_R )
	  {
	      // emit the actual installation progress
	      s_toOCI.emitProgress( progress_R.reportValue() );
	      return true;
	  }
      private:
	  ProgressBar *m_bar;
      };
      
      string outLabel( const string & msg_R ) const
      { return m_labelPrefix.empty() ? msg_R : m_labelPrefix + msg_R; }
  private:
      TriBool m_error;
      ProgressData m_progress;
      string m_progressId;
      string m_labelPrefix;
  };
  
  /** 
   *  \class DownloadProgress
   *  \brief Listens on media::DownloadProgressReport to feed a ProgressBar
   *  
   *  Forward callbacks to any original receiver
   */
  class DownloadProgress : public callback::ReceiveReport<media::DownloadProgressReport>
  {
  public:
      DownloadProgress( ProgressBar & progressBar_R )
      : m_progressBar( &progressBar_R )
      , m_oldReceiver( Distributor::instance().getReceiver() )
      {
	  connect();
      }
      
      ~DownloadProgress()
      {
	  if ( m_oldReceiver )
	      Distributor::instance().setReceiver( *m_oldReceiver );
	  else
	      Distributor::instance().noReceiver();
      }
      
      virtual void start( const Url & file, Pathname localFile )
      {
	  ( *m_progressBar )->range( 100 );	// receives %
	  
	  if ( m_oldReceiver )
	      m_oldReceiver->start( file, localFile );
      }
      
      virtual bool progress( int value, const Url & file, double dbpsAvg = -1, double dbpsCurrent = -1 )
      {
	  ( *m_progressBar )->set( value );
	  
	  if ( m_oldReceiver )
	      return m_oldReceiver->progress( value, file, dbpsAvg, dbpsCurrent );
	  return true;
      }
      
      virtual Action probem( const Url & file, Error error, const string & description )
      {
	  if ( m_oldReceiver )
	      return m_oldReceiver->problem( file, error, description );
	  return Receiver::problem( file, error, description );
      }
      
      virtual void finish( const Url & file, Error error, const string & reason )
      {
	  if ( error == NO_ERROR )
	      ( *m_progressBar )->toMax();
	  else {
	      qDebug() << "Reason [in Download Progress ]" ;
	      m_progressBar->error();
	  }
	  
	  if ( m_oldReceiver )
	      m_oldReceiver->finish( file, error, reason );
      }
      
  private:
      ProgressBar *m_progressBar;
      Receiver *m_oldReceiver;
  };
  
  // progress for installing a resolvable
  class InstallResolvableReportReceiver : public callback::ReceiveReport<target::rpm::InstallResolvableReport>
  {
  public:
      virtual void start( Resolvable::constPtr resolvable )
      {
	  //set current rpm package
	  RuntimeData::instance()->setRpmPackageCurrent( RuntimeData::instance()->rpmPackageCurrent() + 1 );
	  
	  unsigned rpm_pkg_current = RuntimeData::instance()->rpmPackageCurrent();
	  unsigned rpm_pkgs_total = RuntimeData::instance()->rpmPackagesTotal();
	  
	  m_progress.reset( new ProgressBar( "install-resolvable",
					     resolvable->asString(),
					     rpm_pkg_current,
					     rpm_pkgs_total ) );
	  (*m_progress)->range( 100 );	  
	  
	  QString info( QString::fromStdString( "Installing: " + resolvable->asString() ) );
	  s_toOCI.emitStartResolvable( QString::fromStdString( getStartInstallResolvableInfo( resolvable ) ) );
	  s_toOCI.emitStartProgress( info );
      }
      
      virtual bool progress(int value, Resolvable::constPtr resolvable )
      {
	  if ( m_progress )
	      (*m_progress)->set( value );
	  return true;
      }
      
      virtual Action problem( Resolvable::constPtr resolvable, Error error, const string & description, RpmLevel /*unused*/)
      {
	  // finish progress - indicate error
	  if ( m_progress ) {
	      ( *m_progress ).error();
	      m_progress.reset();
	  }
	  
	  qDebug() << "Installation of " << resolvable->asString().c_str() << " failed" ;
	  // emit the problem
	  // emit problemEncountered( resolvable->asString(), description );
	  return (Action) ABORT;
      }
    
      virtual void finish( Resolvable::constPtr resolvable, Error error, const string & reason, RpmLevel /*unused */)
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
		  s_toOCI.emitFinishResolvable( QString::fromStdString( getFinishInstallResolvableInfo( resolvable ) ), error != NO_ERROR );
	  }
	  QString info( QString::fromStdString( "Installed: " + resolvable->asString() ) );
	  s_toOCI.emitFinishProgress( info, error != NO_ERROR );
      }
      
      virtual void reportend()
      { m_progress.reset(); }
      
      string getStartInstallResolvableInfo( Resolvable::constPtr & resolvablePtr )
      {
	  // #006325 -> Dark Green
	  string line( str::Format( "<font color=#006325> ( %s / %s ) </font> <b>Installing :</b> </b><font color=\"Red\">%s</font>" )
				    % RuntimeData::instance()->rpmPackageCurrent()
				    % RuntimeData::instance()->rpmPackagesTotal()
				    % resolvablePtr->asString() );
				    
	  return line;	    
      }
      string getFinishInstallResolvableInfo( Resolvable::constPtr & resolvablePtr)
      {
	  // #14148c -> Dark Blue; 
	  string line( str::Format( "Installed:<font color= #14148c> %s </font> Status:<font color=\"Purple\"> %s </font><br>" )
				    % resolvablePtr->asString()
				    % "Done" );
	  return line;
      }
  private:
      scoped_ptr<ProgressBar> m_progress;
  };
  
  // progress for removing a resolvable
  class RemoveResolvableReportReceiver : public callback::ReceiveReport<target::rpm::RemoveResolvableReport>
  {
  public:
      virtual void start( Resolvable::constPtr resolvable )
      {
	  qDebug() << "Removing " << resolvable->asString().c_str();
	  
	  //set current rpm package
	  RuntimeData::instance()->setRpmPackageCurrent( RuntimeData::instance()->rpmPackageCurrent() + 1 );
	  
	  unsigned rpm_pkg_current = RuntimeData::instance()->rpmPackageCurrent();
	  unsigned rpm_pkgs_total = RuntimeData::instance()->rpmPackagesTotal();
	  
	  m_progress.reset( new ProgressBar( "remove-resolvable",
					     resolvable->asString(),
					     rpm_pkg_current,
					     rpm_pkgs_total ) );
	  ( *m_progress )->range( 100 ); // reports percentage
	  QString info( QString::fromStdString( "<b>Removing: </b>" + resolvable->name() ) );
	  s_toOCI.emitStartResolvable( info );
	  s_toOCI.emitStartProgress( info );
      }
      
      virtual bool progress( int value, Resolvable::constPtr resolvable )
      {
	  if ( m_progress )
	      ( *m_progress )->set( value );
	  return true;
      }
      
      virtual Action problem( Resolvable::constPtr resolvable, Error error, const string & description )
      {
	  // finish progress - indicate error
	  if ( m_progress ) {
	      ( *m_progress ).error();
	      m_progress.reset();
	  }
	  
	  qDebug() << "Removal of " << resolvable->asString().c_str() << " failed" ;
	  // emit the problem
	  // emit problemEncountered( resolvable->asString(), description );
	  return (Action) ABORT;
      }
      
      virtual void finish( Resolvable::constPtr resolvable, Error error, const string & reason )
      {
	  QString info( QString::fromStdString( "<b>Removed: </b>" + resolvable->name() ) );
	  // finish progress - indicate error
	  if ( m_progress ) {
	      ( *m_progress ).error( error != NO_ERROR );
	      m_progress.reset();
	  }
	  
	  if ( error != NO_ERROR ) {
	      // quit the helper and OCI
	  }
	  else {
	      if ( !reason.empty() ) ;
		  s_toOCI.emitFinishResolvable( info, error != NO_ERROR );
	  }
	  s_toOCI.emitFinishProgress( info, error != NO_ERROR );
      }
      
      virtual void reportend()
      { m_progress.reset(); }
  private:
      scoped_ptr<ProgressBar> m_progress;
  };
  
  class FindFileConflictsReportReceiver : public callback::ReceiveReport<target::FindFileConflictstReport>
  {
  public:
      virtual void reportbegin()
      {
	  m_progress.reset( new ProgressBar( "fileconflict-check",
					     // progressbar label
					     "Checking for file conflicts:" ) );
      }
      
      virtual bool start( const ProgressData & progress_R )
      {
	  ( *m_progress )->set( progress_R );
	  // return !OCI::instance()->exitRequested(); //refer to line 88 (media.h)
	  QString info( QString::fromStdString( "<font color=\"Red\"><b>Checking For File Conflicts...</b></font>" ) );
	  s_toOCI.emitStartResolvable( info );
	  s_toOCI.emitStartProgress( info );
	  return true; //this should suffice for now. Temporary fix
      }
      
      virtual bool progress( const ProgressData & progress_R, const sat::Queue & noFilelist_R )
      {
	  ( *m_progress )->set( progress_R );
	  // return !OCI::instance()->exitRequested(); //refer to line 88 (media.h)
	  return true; //this should suffice for now. Temporary fix
      }
      
      virtual bool result( const ProgressData & progress_R, const sat::Queue & noFilelist_R, const sat::FileConflicts & conflicts_R )
      {
	  // finish progress - only conflicts count as error
	  ( *m_progress ).error( !conflicts_R.empty() );
	  m_progress.reset();
	  
	  if ( conflicts_R.empty() && noFilelist_R.empty() ) {
	      //s_toOCI.emitFinishResolvable( true );
	      return true;// !OCI::instance()->exitRequested();
	  }
	  
	  // show error result
	  if ( !noFilelist_R.empty() ) //warning
	  {
	      qDebug() << "Checking for file conflicts requires not installed packages to be downloaded in advance "
	              "in order to access their file lists." ;
	      // print them out
	      qDebug() << "The following packages had to be excluded from file conflicts check because they are not yet downloaded" ;
	      for_( it, noFilelist_R.begin(), noFilelist_R.end() )
		  ;//qDebug() << ( *it );
	  }
	  
	  if ( !conflicts_R.empty() )
	  {
	      // use i18np() here
	      qDebug() << "Detected " << conflicts_R.size() << "file conflict(s)" ;
	      //print them out
	      for_( it, conflicts_R.begin(), conflicts_R.end() )
		  ;//qDebug() << ( *it ) ;
	      
	      // prompt general info (for now) about why file conflicts usually occur
	      bool cont = true;
	      string generalInfo = "File conflicts happen when two packages attempt to install files with the same name but different contents. If you continue, conflicting files will be replaced losing the previous content. Continue? [yes/no]";
	      int reply = KMessageBox::questionYesNo( 0, QString::fromStdString( generalInfo ), "File Conflicts!" );
	      if ( reply == KMessageBox::ButtonCode::No )
		  cont = false;
	      
	      if ( !cont )
		  return false;	// aborted
	  }
	  return true; //!OCI::instance()->exitRequested();
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

class RpmCallbacks 
{
public:
    RpmCallbacks()
    {
	m_installReceiver.connect();
	m_removeReceiver.connect();
	m_fileConflictsReceiver.connect();
    }
    ~RpmCallbacks()
    {
	m_installReceiver.disconnect();
	m_removeReceiver.disconnect();
	m_fileConflictsReceiver.disconnect();
    }
private:
    OCICallbacks::InstallResolvableReportReceiver m_installReceiver;
    OCICallbacks::RemoveResolvableReportReceiver m_removeReceiver;
    OCICallbacks::FindFileConflictsReportReceiver m_fileConflictsReceiver;
};
#endif
