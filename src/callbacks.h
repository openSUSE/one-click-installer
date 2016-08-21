#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <QObject> 

class Callbacks : public QObject
{
    Q_OBJECT
    Q_CLASSINFO( "D-Bus Interface", "org.opensuse.MediaCallbacks" )
public:
    Callbacks();
    void emitStartResolvable( QString info );
    void emitFinishResolvable( QString info, bool success );
    void emitProgress( int value );
    void emitStartDownload( QString );
    void emitFinishDownload( QString, bool success );
Q_SIGNALS:
    void startDownload( QString );
    void finishDownload( QString, bool );
    void startResolvable( QString );
    void finishResolvable( QString, bool );
    void progress( int );
};
#endif
