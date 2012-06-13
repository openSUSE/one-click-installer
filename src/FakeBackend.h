#ifndef FAKEBACKEND_H
#define FAKEBACKEND_H

#include "PackageBackend.h"
#include <QList>
#include <QString>
#include <QWidget>
#include <QMessageBox>

class FakeBackend : public PackageBackend
{
    public:
    FakeBackend( QWidget* main );
    virtual void addRepository( const QString& url  );
    virtual bool performInstallation( const QList< QString >& pack  );
    virtual bool resolveConflicts();
    bool m_allTrusted;

    private:
    QWidget *m_main;
};

#endif
