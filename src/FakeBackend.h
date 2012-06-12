#ifndef FAKEBACKEND_H
#define FAKEBACKEND_H

#include "PackageBackend.h"
#include <QList>
#include <QString>

class FakeBackend : public PackageBackend
{
    public:
    FakeBackend();
    virtual void addRepository( const QString& url  );
    virtual bool performInstallation();
    virtual bool resolveConflicts();

    private:
    QList< QString > packageList;
    QList< QString > repoList;
    bool m_allTrusted;
};

#endif
