#ifndef BACKEND_H
#define BACKEND_H

#include "PackageBackend.h"

class Backend : public PackageBackend
{
public:
    Backend();
    void install();
};

#endif
