#ifndef ABSTRACTBACKEND_H
#define ABSTRACTBACKEND_H

#include <QList>

class AbstractBackend
{
    public:
    virtual void parseYmpFile() = 0;
    virtual void addRepository() = 0;
    virtual bool performInstallation() = 0;
    virtual bool resolveConflicts() = 0;
    virtual QList< QString > packages() = 0;
    virtual QList< QString > repos() = 0;
};

#endif
