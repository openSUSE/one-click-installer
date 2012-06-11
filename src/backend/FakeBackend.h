#ifndef FAKEBACKEND_H
#define FAKEBACKEND_H

#include "AbstractBackend.h"
#include <QList>
#include <QString>

class FakeBackend : public AbstractBackend
{
    public:
    FakeBackend();
    virtual void parseYmpFile();
    virtual void addRepository();
    virtual bool performInstallation();
    virtual bool resolveConflicts();
    virtual QList< QString > repos();
    virtual QList< QString > packages();

    private:
    QList< QString > packageList;
    QList< QString > repoList;
    bool m_allTrusted;
};

#endif
