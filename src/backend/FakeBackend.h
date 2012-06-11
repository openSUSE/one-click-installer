#ifndef FAKEBACKEND_H
#define FAKEBACKEND_H

#include "FakeBackend.h"

class FakeBackend : public AbstractBackend
{
    public:
    void parseYmpFile();
    void addRepository();
    bool performInstallation();
    bool resolveConflicts();
};

#define
