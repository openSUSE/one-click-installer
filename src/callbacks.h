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
    void emitFinishResolvable( bool success );
    void emitProgress( int value );
Q_SIGNALS:
    void startResolvable( QString );
    void finishResolvable( bool );
    void progress( int );
};
#endif
