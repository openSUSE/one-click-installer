#ifndef INSTALLTEST_H
#define INSTALLTEST_H

#include <QtTest/QTest>
#include <zypp/ZYppFactory.h>
#include <zypp/misc/DefaultLoadSystem.h>

#include "../../src/backend/package.h"


class InstallTest : public QObject
{
    Q_OBJECT
    private slots:
        void installTest();

};
#endif
