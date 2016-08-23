#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <QObject> 

class Callbacks : public QObject
{
    Q_OBJECT
    Q_CLASSINFO( "D-Bus Interface", "org.opensuse.MediaCallbacks" )
public:
    Callbacks();
    void emitStartResolvable( const QString & info );
    void emitFinishResolvable( const QString & info, bool success );
    void emitProgress( int value );
    void emitStartProgress( const QString & info);
    void emitFinishProgress( const QString & info, bool success );
Q_SIGNALS:
    void startProgress( QString );
    void finishProgress( QString, bool );
    void startResolvable( QString );
    void finishResolvable( QString, bool );
    void progress( int );
};
#endif
