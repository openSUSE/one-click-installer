#ifndef KEYRINGTEST_H
#define KEYRINGTEST_H

#include <QObject>
#include "repository.h"
#include "keyringcallbacks.h"

class KeyRingTest : public QObject
{
    Q_OBJECT
        private slots:
        void checkKeyRing();
};

#endif
