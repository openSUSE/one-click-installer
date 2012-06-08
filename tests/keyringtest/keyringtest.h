#ifndef KEYRINGTEST_H
#define KEYRINGTEST_H

#include <QObject>
#include "../../src/backend/repository.h"
#include "../../src/backend/keyringcallbacks.h"

class KeyRingTest : public QObject
{
    Q_OBJECT
        private slots:
        void checkKeyRing();
};

#endif
